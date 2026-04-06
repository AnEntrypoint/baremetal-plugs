# CLAUDE.md — baremetal-vital

## Non-obvious technical caveats

### Vital LV2 build is NOT headless
The Vital LV2 plugin links `-lGL` and compiles `include_juce_opengl.cpp` even for DSP-only builds.
Do not attempt to use the LV2 plugin path. Use `src/vital_synth.h/.cpp` which wraps
`vital::SoundEngine` directly from the headless synthesis sources.

### Output buffer stride is poly_float::kSize, not 1
On ARM with NEON, `poly_float::kSize = 4`. Stereo output is at indices
`[kSize*i + 0]` (left) and `[kSize*i + 1]` (right) — not `[i*2+0]` / `[i*2+1]`.

### STDLIB_SUPPORT = 3 is required for Vital
Circle defaults to `STDLIB_SUPPORT=1` (no libstdc++). Vital's synthesis code uses
STL containers extensively. Set `STDLIB_SUPPORT=3` in the Makefile to link libstdc++
and enable RTTI and exceptions.

### Vital sources: synthesis/ and common/ only
Include only:
- `src/synthesis/**/*.cpp`
- `src/common/*.cpp`

Do not include `src/plugin/`, `src/interface/`, `src/headless/main.cpp`,
or any JUCE module that pulls in `juce_audio_devices` or `juce_gui_basics`.

### Circle USB audio: Write-only, queue-fill pattern
`CUSBSoundBaseDevice` uses a write-ahead queue (not a GetChunk callback).
Pattern: `AllocateQueue(ms)` → `SetWriteFormat(SoundFormatSigned16, 2)` → `Start()` →
loop: `MsSleep` + `Write(buf, bytes)`. The queue drains autonomously over USB ISO.

### USBHCI must initialize before sound device
`CUSBHCIDevice::Initialize()` must complete before `new CUSBSoundBaseDevice()` is called.
Constructing the sound device before USB stack is ready = silent failure.

### Target binary name for Pi 4 aarch64
With `RASPPI=4 AARCH=64`, Circle produces `kernel8-rpi4.img` (not `kernel8.img`).
Copy this as `kernel8-rpi4.img` to the SD card FAT32 partition alongside `config.txt`.
