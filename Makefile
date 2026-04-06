RASPPI    ?= 4
AARCH     ?= 64
CIRCLEHOME = ../circle
VITALHOME  = ../vital

STDLIB_SUPPORT = 3

VITALINC = \
    -I$(VITALHOME)/src/common \
    -I$(VITALHOME)/src/common/wavetable \
    -I$(VITALHOME)/src/synthesis \
    -I$(VITALHOME)/src/synthesis/synth_engine \
    -I$(VITALHOME)/src/synthesis/effects \
    -I$(VITALHOME)/src/synthesis/effects_engine \
    -I$(VITALHOME)/src/synthesis/filters \
    -I$(VITALHOME)/src/synthesis/framework \
    -I$(VITALHOME)/src/synthesis/lookups \
    -I$(VITALHOME)/src/synthesis/modulators \
    -I$(VITALHOME)/src/synthesis/modules \
    -I$(VITALHOME)/src/synthesis/producers \
    -I$(VITALHOME)/src/synthesis/utilities \
    -I$(VITALHOME)/src/interface/look_and_feel \
    -I$(VITALHOME)/third_party/JUCE/modules \
    -I$(VITALHOME)/third_party

VITALDEFS = -DHEADLESS=1 -DNO_AUTH=1 -DJUCE_STANDALONE_APPLICATION=0 \
    -DJUCE_USE_CURL=0 -DJUCE_PROJUCER_VERSION=0x60005 -DLINUX=1

VITALFLAGS = -I src $(VITALINC) $(VITALDEFS) -include operators.h -include chorus_module.h

EXTRAINCLUDE += -I src $(VITALINC) -include operators.h -include chorus_module.h
DEFINE       += $(VITALDEFS)

VITAL_COMMON_SRCS := \
    $(VITALHOME)/src/common/line_generator.cpp \
    $(VITALHOME)/src/common/synth_parameters.cpp \
    $(VITALHOME)/src/common/synth_types.cpp \
    $(VITALHOME)/src/common/tuning.cpp
VITAL_SYNTH_SRCS  := $(wildcard $(VITALHOME)/src/synthesis/synth_engine/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/effects/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/effects_engine/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/filters/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/framework/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/lookups/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/modulators/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/modules/*.cpp) \
    $(wildcard $(VITALHOME)/src/synthesis/producers/*.cpp) \
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

src/vital_synth.o: src/vital_synth.cpp
	@echo "  CPP   $@"
	@$(CPP) $(CPPFLAGS) $(VITALFLAGS) -c -o $@ $<

$(VITAL_OBJS): %.o: %.cpp
	@echo "  CPP   $@"
	@$(CPP) $(CPPFLAGS) $(VITALFLAGS) -c -o $@ $<

-include $(DEPS)
