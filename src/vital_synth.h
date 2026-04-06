#pragma once
#include <circle/types.h>

struct VitalSynthImpl;

class VitalSynth {
public:
    static constexpr int kBufferSize = 64;
    static constexpr int kSampleRate = 48000;

    VitalSynth();
    ~VitalSynth();

    void init();
    void noteOn(int note, float velocity);
    void noteOff(int note);
    void fillBuffer(s16* pBuffer, unsigned nFrames);

private:
    VitalSynthImpl* impl_;
};
