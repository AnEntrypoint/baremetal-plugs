#pragma once

#include "sound_engine.h"
#include "synth_types.h"

class VitalSynth {
public:
    static constexpr int kBufferSize = 64;
    static constexpr int kSampleRate = 48000;

    VitalSynth();
    ~VitalSynth() = default;

    void init();
    void noteOn(int note, float velocity);
    void noteOff(int note);
    void fillBuffer(s16* pBuffer, unsigned nFrames);

private:
    vital::SoundEngine engine_;
    bool initialized_;
};
