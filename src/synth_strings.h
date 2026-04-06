#pragma once
// Forward declare vital::StereoEncoder before the real synth_strings.h uses it
namespace vital {
  class StereoEncoder {
  public:
    enum { kNumStereoModes = 2 };
  };
}
#include_next "synth_strings.h"
