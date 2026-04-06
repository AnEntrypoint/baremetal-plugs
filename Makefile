RASPPI    ?= 4
AARCH     ?= 64
CIRCLEHOME = ../circle
VITALHOME  = ../vital

STDLIB_SUPPORT = 3

VITALINC = \
    -I$(VITALHOME)/src/common \
    -I$(VITALHOME)/src/synthesis \
    -I$(VITALHOME)/src/synthesis/synth_engine \
    -I$(VITALHOME)/src/synthesis/effects \
    -I$(VITALHOME)/src/synthesis/filters \
    -I$(VITALHOME)/src/synthesis/framework \
    -I$(VITALHOME)/src/synthesis/lookups \
    -I$(VITALHOME)/src/synthesis/modulators \
    -I$(VITALHOME)/src/synthesis/modules \
    -I$(VITALHOME)/src/synthesis/utilities \
    -I$(VITALHOME)/third_party/JUCE/modules \
    -I$(VITALHOME)/third_party/json

VITALDEFS = -DHEADLESS=1 -DNO_AUTH=1 -DJUCE_GLOBAL_MODULE_SETTINGS_INCLUDED=1 \
    -DJUCE_STANDALONE_APPLICATION=0 \
    -DJUCE_MODULE_AVAILABLE_juce_audio_basics=1 \
    -DJUCE_MODULE_AVAILABLE_juce_audio_formats=1 \
    -DJUCE_MODULE_AVAILABLE_juce_core=1 \
    -DJUCE_MODULE_AVAILABLE_juce_data_structures=1 \
    -DJUCE_MODULE_AVAILABLE_juce_dsp=1 \
    -DJUCE_MODULE_AVAILABLE_juce_events=1

EXTRAINCLUDE += $(VITALINC) -I src
DEFINE       += $(VITALDEFS)

VITAL_COMMON_SRCS := $(wildcard $(VITALHOME)/src/common/*.cpp)
VITAL_SYNTH_SRCS  := $(wildcard $(VITALHOME)/src/synthesis/synth_engine/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/effects/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/filters/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/framework/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/lookups/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/modulators/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/modules/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/utilities/*.cpp)

VITAL_OBJS := $(VITAL_COMMON_SRCS:.cpp=.o) $(VITAL_SYNTH_SRCS:.cpp=.o)

OBJS = src/main.o src/kernel.o src/vital_synth.o $(VITAL_OBJS)

LIBS = $(CIRCLEHOME)/lib/sched/libsched.a \
       $(CIRCLEHOME)/lib/sound/libsound.a \
       $(CIRCLEHOME)/lib/usb/libusb.a \
       $(CIRCLEHOME)/lib/input/libinput.a \
       $(CIRCLEHOME)/lib/fs/libfs.a \
       $(CIRCLEHOME)/lib/libcircle.a

include $(CIRCLEHOME)/Rules.mk

-include $(DEPS)
