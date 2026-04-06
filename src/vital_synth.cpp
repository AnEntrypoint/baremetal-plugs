#include "vital_synth.h"
#include "processor.h"
#include <cmath>
#include <algorithm>

VitalSynth::VitalSynth() : initialized_(false) {}

void VitalSynth::init() {
    engine_.setSampleRate(kSampleRate);
    engine_.init();
    engine_.setBpm(120.0f);
    initialized_ = true;
}

void VitalSynth::noteOn(int note, float velocity) {
    if (!initialized_) return;
    engine_.noteOn(note, velocity, 0, 0);
}

void VitalSynth::noteOff(int note) {
    if (!initialized_) return;
    engine_.noteOff(note, 1.0f, 0, 0);
}

void VitalSynth::fillBuffer(s16* pBuffer, unsigned nFrames) {
    if (!initialized_) {
        for (unsigned i = 0; i < nFrames * 2; ++i) pBuffer[i] = 0;
        return;
    }

    unsigned offset = 0;
    while (offset < nFrames) {
        unsigned block = std::min((unsigned)kBufferSize, nFrames - offset);
        engine_.process(block);

        const vital::mono_float* out =
            (const vital::mono_float*)engine_.output(0)->buffer;

        for (unsigned i = 0; i < block; ++i) {
            float l = out[vital::poly_float::kSize * i + 0];
            float r = out[vital::poly_float::kSize * i + 1];
            l = std::max(-1.0f, std::min(1.0f, l));
            r = std::max(-1.0f, std::min(1.0f, r));
            pBuffer[(offset + i) * 2 + 0] = (s16)(l * 32767.0f);
            pBuffer[(offset + i) * 2 + 1] = (s16)(r * 32767.0f);
        }
        offset += block;
    }
}
