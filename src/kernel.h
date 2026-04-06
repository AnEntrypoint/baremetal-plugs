#pragma once

#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/sched/scheduler.h>
#include <circle/usb/usbhcidevice.h>
#include <circle/sound/soundbasedevice.h>
#include <circle/types.h>
#include "vital_synth.h"

enum TShutdownMode { ShutdownNone, ShutdownHalt, ShutdownReboot };

class CKernel {
public:
    CKernel();
    ~CKernel();

    boolean Initialize();
    TShutdownMode Run();

private:
    void WriteSoundData(unsigned nFrames);

private:
    CActLED             m_ActLED;
    CKernelOptions      m_Options;
    CDeviceNameService  m_DeviceNameService;
    CScreenDevice       m_Screen;
    CSerialDevice       m_Serial;
    CExceptionHandler   m_ExceptionHandler;
    CInterruptSystem    m_Interrupt;
    CTimer              m_Timer;
    CLogger             m_Logger;
    CScheduler          m_Scheduler;
    CUSBHCIDevice       m_USBHCI;
    CSoundBaseDevice*   m_pSound;
    VitalSynth          m_Synth;
};
