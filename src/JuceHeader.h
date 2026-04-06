#pragma once

// Bare-metal stub replacing juce_core. Provides STL includes and minimal
// JUCE type stubs so Vital synthesis sources compile without juce_core POSIX deps.

#include <algorithm>
#include <atomic>
#include <complex>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <random>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// JUCE integer typedefs
using int8   = int8_t;
using uint8  = uint8_t;
using int16  = int16_t;
using uint16 = uint16_t;
using int32  = int32_t;
using uint32 = uint32_t;
using int64  = int64_t;
using uint64 = uint64_t;

// Minimal juce::String stub (non-functional, compile-only)
class String {
public:
    String() {}
    String(const char* s) : s_(s ? s : "") {}
    String(const std::string& s) : s_(s) {}
    String operator+(const String& o) const { return String(s_ + o.s_); }
    String operator+(const char* o) const { return String(s_ + (o ? o : "")); }
    bool operator==(const String& o) const { return s_ == o.s_; }
    bool operator!=(const String& o) const { return s_ != o.s_; }
    bool operator<(const String& o) const { return s_ < o.s_; }
    String toLowerCase() const { auto r = s_; for(auto& c:r) c=tolower(c); return String(r); }
    String trim() const { return String(s_); }
    String upToFirstOccurrenceOf(const String&, bool, bool) const { return *this; }
    String fromLastOccurrenceOf(const String&, bool, bool) const { return *this; }
    String removeCharacters(const String&) const { return *this; }
    bool startsWith(const String& p) const { return s_.find(p.s_) == 0; }
    bool endsWith(const String& p) const { return s_.size() >= p.s_.size() && s_.rfind(p.s_) == s_.size()-p.s_.size(); }
    bool contains(const String& p) const { return s_.find(p.s_) != std::string::npos; }
    bool isEmpty() const { return s_.empty(); }
    bool isNotEmpty() const { return !s_.empty(); }
    int length() const { return (int)s_.size(); }
    const char* toRawUTF8() const { return s_.c_str(); }
    std::string toStdString() const { return s_; }
    static String fromUTF8(const char* s) { return String(s); }
    float getFloatValue() const { return s_.empty() ? 0.0f : (float)std::atof(s_.c_str()); }
    int getIntValue() const { return s_.empty() ? 0 : std::atoi(s_.c_str()); }
    String substring(int start, int end = -1) const {
        if (end < 0) return String(s_.substr(start));
        return String(s_.substr(start, end - start));
    }
    char operator[](int i) const { return (i >= 0 && i < (int)s_.size()) ? s_[i] : 0; }
    String fromFirstOccurrenceOf(const String& sub, bool includeSubString, bool) const { 
        auto pos = s_.find(sub.s_);
        if (pos == std::string::npos) return String();
        return String(s_.substr(includeSubString ? pos : pos + sub.s_.size()));
    }
private:
    std::string s_;
};
inline String operator+(const char* a, const String& b) { return String(a) + b; }

// StringArray stub
class StringArray {
public:
    StringArray() {}
    void add(const String& s) { v_.push_back(s); }
    int size() const { return (int)v_.size(); }
    const String& operator[](int i) const { return v_[i]; }
    auto begin() const { return v_.begin(); }
    auto end() const { return v_.end(); }
    static StringArray fromLines(const String& s) { StringArray sa; return sa; }
    static StringArray fromTokens(const String&, const String&, const String&) { return StringArray(); }
    static StringArray fromTokens(const String&, bool) { return StringArray(); }
    int addTokens(const String& str, const String& delims, const String& = String()) {
        std::string src = str.toStdString();
        std::string d = delims.toStdString();
        size_t start = 0, end;
        while ((end = src.find_first_of(d, start)) != std::string::npos) {
            if (end > start) v_.push_back(String(src.substr(start, end - start)));
            start = end + 1;
        }
        if (start < src.size()) v_.push_back(String(src.substr(start)));
        return (int)v_.size();
    }
    int addTokens(const String& str, bool) {
        return addTokens(str, String(" \t"));
    }
private:
    std::vector<String> v_;
};

// File stub (bare-metal: no filesystem)
class InputStream { public: virtual ~InputStream(){} virtual bool isExhausted(){return true;} virtual int read(void*,int){return 0;} virtual String readNextLine(){return String();} virtual bool isEOF(){return true;} };
class File {
public:
    File() {}
    File(const String& path) : path_(path) {}
    bool existsAsFile() const { return false; }
    bool isDirectory() const { return false; }
    String getFileExtension() const { return String(); }
    String getFullPathName() const { return path_; }
    String getFileName() const { return path_; }
    String getFileNameWithoutExtension() const { 
        auto s = path_.toStdString();
        auto slash = s.rfind('/');
        if (slash != std::string::npos) s = s.substr(slash+1);
        auto dot = s.rfind('.');
        if (dot != std::string::npos) s = s.substr(0, dot);
        return String(s);
    }
    File getChildFile(const String&) const { return File(); }
    File getParentDirectory() const { return File(); }
    StringArray readLines() const { return StringArray(); }
    bool readLines(StringArray& result) const { return false; }
    std::unique_ptr<InputStream> createInputStream() const { return nullptr; }
    bool operator==(const File& o) const { return path_ == o.path_; }
    bool operator!=(const File& o) const { return path_ != o.path_; }
    bool operator<(const File& o) const { return path_ < o.path_; }
    static File getSpecialLocation(int) { return File(); }
    static File getCurrentWorkingDirectory() { return File(); }
    bool isNull() const { return path_.isEmpty(); }
private:
    String path_;
};

// MidiMessage stub
class MidiMessage {
public:
    MidiMessage() : data_{0,0,0,0} {}
    bool isNoteOn(bool=true) const { return false; }
    bool isNoteOff(bool=true) const { return false; }
    bool isController() const { return false; }
    bool isSustainPedalOn() const { return false; }
    bool isSostenutoPedalOn() const { return false; }
    bool isChannelPressure() const { return false; }
    bool isAftertouch() const { return false; }
    bool isPitchWheel() const { return false; }
    int getNoteNumber() const { return 0; }
    int getVelocity() const { return 0; }
    float getFloatVelocity() const { return 0.0f; }
    int getControllerNumber() const { return 0; }
    int getControllerValue() const { return 0; }
    int getChannel() const { return 1; }
    int getPitchWheelValue() const { return 0; }
    int getChannelPressureValue() const { return 0; }
    int getAfterTouchValue() const { return 0; }
    const uint8* getRawData() const { return data_; }
    int getRawDataSize() const { return 4; }
    double getTimeStamp() const { return 0.0; }
private:
    uint8 data_[4];
};

// MidiBuffer stub
class MidiBuffer {
public:
    struct Iterator {
        Iterator(const MidiBuffer&) {}
        bool getNextEvent(MidiMessage&, int&) { return false; }
    };
    bool isEmpty() const { return true; }
    void clear() {}
    void addEvent(const MidiMessage&, int) {}
};

// MidiKeyboardState stub
class MidiKeyboardState {
public:
    void reset() {}
    void processNextMidiBuffer(MidiBuffer&, int, int, bool) {}
};



// MemoryOutputStream stub
class MemoryOutputStream {
public:
    explicit MemoryOutputStream(size_t initialSize = 0) {}
    const void* getData() const { return nullptr; }
    size_t getDataSize() const { return 0; }
    bool write(const void*, size_t) { return true; }
};

// Base64 stub (bare-metal: no encode/decode needed)
struct Base64 {
    static String toBase64(const void*, size_t) { return String(); }
    static bool convertFromBase64(MemoryOutputStream&, const String&) { return false; }
};

// FloatVectorOperations stub (juce_audio_basics)
class FloatVectorOperations {
public:
    static void disableDenormalisedNumberSupport(bool=true) {}
    static void enableFlushToZeroMode(bool=true) {}
};
// JUCE macros
#define JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(className) \
    className(const className&) = delete; \
    className& operator=(const className&) = delete;

#define JUCE_LEAK_DETECTOR(className)

#define JUCE_MODULE_AVAILABLE_juce_audio_basics    1
#define JUCE_MODULE_AVAILABLE_juce_audio_formats   1
#define JUCE_MODULE_AVAILABLE_juce_core            1
#define JUCE_MODULE_AVAILABLE_juce_data_structures 1
#define JUCE_MODULE_AVAILABLE_juce_dsp             0
#define JUCE_MODULE_AVAILABLE_juce_events          1
#define JUCE_GLOBAL_MODULE_SETTINGS_INCLUDED       1
#define JUCE_STANDALONE_APPLICATION               0
