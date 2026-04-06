#pragma once

#define JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(className) \
    className(const className&) = delete; \
    className& operator=(const className&) = delete;

#define JUCE_LEAK_DETECTOR(className)

#define JUCE_MODULE_AVAILABLE_juce_audio_basics    1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats   1
#define JUCE_MODULE_AVAILABLE_juce_core            1
#define JUCE_MODULE_AVAILABLE_juce_data_structures 1
#define JUCE_MODULE_AVAILABLE_juce_dsp             1
#define JUCE_MODULE_AVAILABLE_juce_events          1
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED       1
#define JUCE_STANDALONE_APPLICATION               0
