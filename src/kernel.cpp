#include "kernel.h"
#include "config.h"
#include <circle/sound/usbsoundbasedevice.h>
#include <circle/util.h>
#include <assert.h>

static const char FromKernel[] = "kernel";

CKernel::CKernel()
:   m_Screen(m_Options.GetWidth(), m_Options.GetHeight()),
    m_Timer(&m_Interrupt),
    m_Logger(m_Options.GetLogLevel(), &m_Timer),
    m_USBHCI(&m_Interrupt, &m_Timer, FALSE),
    m_pSound(0)
{
    m_ActLED.Blink(5);
}

CKernel::~CKernel() {}

boolean CKernel::Initialize() {
    boolean bOK = TRUE;
    if (bOK) bOK = m_Screen.Initialize();
    if (bOK) bOK = m_Serial.Initialize(115200);
    if (bOK) {
        CDevice* pTarget = m_DeviceNameService.GetDevice(m_Options.GetLogDevice(), FALSE);
        if (!pTarget) pTarget = &m_Screen;
        bOK = m_Logger.Initialize(pTarget);
    }
    if (bOK) bOK = m_Interrupt.Initialize();
    if (bOK) bOK = m_Timer.Initialize();
    if (bOK) bOK = m_USBHCI.Initialize();
    return bOK;
}

TShutdownMode CKernel::Run() {
    m_Logger.Write(FromKernel, LogNotice, "Compile time: " __DATE__ " " __TIME__);

    m_pSound = new CUSBSoundBaseDevice(SAMPLE_RATE);
    assert(m_pSound != 0);

    m_Synth.init();
    m_Logger.Write(FromKernel, LogNotice, "Vital initialized");

    m_Synth.noteOn(60, 0.8f);

    if (!m_pSound->AllocateQueue(QUEUE_SIZE_MSECS)) {
        m_Logger.Write(FromKernel, LogPanic, "Cannot allocate sound queue");
    }

    m_pSound->SetWriteFormat(SoundFormatSigned16, WRITE_CHANNELS);

    unsigned nQueueSizeFrames = m_pSound->GetQueueSizeFrames();
    WriteSoundData(nQueueSizeFrames);

    if (!m_pSound->Start()) {
        m_Logger.Write(FromKernel, LogPanic, "Cannot start sound device");
    }

    m_Logger.Write(FromKernel, LogNotice, "USB audio started");

    for (unsigned nCount = 0; m_pSound->IsActive(); nCount++) {
        m_Scheduler.MsSleep(QUEUE_SIZE_MSECS / 2);
        WriteSoundData(nQueueSizeFrames - m_pSound->GetQueueFramesAvail());
        m_Screen.Rotor(0, nCount);
    }

    return ShutdownHalt;
}

void CKernel::WriteSoundData(unsigned nFrames) {
    const unsigned nFramesPerWrite = CHUNK_SIZE;
    s16 Buffer[nFramesPerWrite * WRITE_CHANNELS];

    while (nFrames > 0) {
        unsigned nWrite = nFrames < nFramesPerWrite ? nFrames : nFramesPerWrite;
        m_Synth.fillBuffer(Buffer, nWrite);
        unsigned nBytes = nWrite * WRITE_CHANNELS * sizeof(s16);
        int nResult = m_pSound->Write(Buffer, nBytes);
        if (nResult != (int)nBytes) {
            m_Logger.Write(FromKernel, LogError, "Sound data dropped");
        }
        nFrames -= nWrite;
        m_Scheduler.Yield();
    }
}
