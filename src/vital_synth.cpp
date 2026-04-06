#include "vital_synth.h"
#include "sound_engine.h"
#include "synth_types.h"
#include "processor.h"
#include <cmath>
#include <algorithm>

struct VitalSynthImpl {
    vital::SoundEngine engine;
    bool initialized = false;
};

VitalSynth::VitalSynth() : impl_(new VitalSynthImpl()) {}

VitalSynth::~VitalSynth() { delete impl_; }

void VitalSynth::init() {
    impl_->engine.setSampleRate(kSampleRate);
    impl_->engine.init();
    impl_->engine.setBpm(120.0f);
    impl_->initialized = true;
}

void VitalSynth::noteOn(int note, float velocity) {
    if (!impl_->initialized) return;
    impl_->engine.noteOn(note, velocity, 0, 0);
}

void VitalSynth::noteOff(int note) {
    if (!impl_->initialized) return;
    impl_->engine.noteOff(note, 1.0f, 0, 0);
}

void VitalSynth::fillBuffer(s16* pBuffer, unsigned nFrames) {
    if (!impl_->initialized) {
        for (unsigned i = 0; i < nFrames * 2; ++i) pBuffer[i] = 0;
        return;
    }
    unsigned offset = 0;
    while (offset < nFrames) {
        unsigned block = std::min((unsigned)kBufferSize, nFrames - offset);
        impl_->engine.process(block);
        const vital::mono_float* out =
            (const vital::mono_float*)impl_->engine.output(0)->buffer;
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
