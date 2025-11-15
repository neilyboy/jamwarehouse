# PlatformIO Ubuntu APT Package Notes

## Version Compatibility

The Ubuntu apt repository contains PlatformIO **v4.3.4** (released ~2020), which is significantly older than the current version (v6.x).

### What Works
✅ Building and uploading firmware  
✅ Serial monitoring  
✅ Library management via GitHub URLs  
✅ All core functionality  

### Limitations of Apt Version
- ❌ No `src_dir` support (must build from firmware directory)
- ❌ Some newer registry library references don't work
- ❌ Missing some newer features like `monitor_filters`

### Workaround Applied
The `build.sh` script now automatically:
1. Copies `platformio.ini` to `firmware/JamWysteria/`
2. Runs build commands from the firmware directory
3. Uses GitHub URLs for all ESP32-specific libraries

## Upgrading to Latest PlatformIO (Optional)

If you want the latest PlatformIO features, you can upgrade manually:

### Method 1: Using pipx (Recommended)
```bash
# Remove apt version
sudo apt remove platformio

# Install pipx
sudo apt install pipx
pipx ensurepath

# Install latest PlatformIO
pipx install platformio

# Restart terminal
source ~/.bashrc

# Verify
pio --version  # Should show 6.x.x
```

### Method 2: Using Python venv
```bash
# Remove apt version
sudo apt remove platformio

# Create virtual environment
python3 -m venv ~/.platformio-venv

# Activate it
source ~/.platformio-venv/bin/activate

# Install latest
pip install platformio

# Add to ~/.bashrc for permanent access
echo 'source ~/.platformio-venv/bin/activate' >> ~/.bashrc
```

### Method 3: Keep Apt Version
The apt version works fine! Just stick with it if you don't need the latest features.

## Current Configuration

Your `platformio.ini` is now configured for maximum compatibility:
- ✅ Works with both old and new PlatformIO versions
- ✅ Uses GitHub URLs for all problematic libraries
- ✅ Build script handles directory structure automatically

## Testing Your Setup

```bash
# Clean any previous builds
./build.sh clean

# Install dependencies
./build.sh deps

# Build firmware
./build.sh build

# If successful, upload to ESP32
./build.sh upload
```

## Library References Fixed

| Library | Old (Registry) | New (GitHub) |
|---------|---------------|--------------|
| ESP32-audioI2S | `schreibfaul1/ESP32-audioI2S@^3.0.0` | `https://github.com/schreibfaul1/ESP32-audioI2S.git` |
| ESPAsyncWebServer | `me-no-dev/ESPAsyncWebServer@^1.2.3` | `https://github.com/me-no-dev/ESPAsyncWebServer.git` |
| AsyncTCP | `me-no-dev/AsyncTCP@^1.1.1` | `https://github.com/me-no-dev/AsyncTCP.git` |
| XPT2046_Touchscreen | `paulstoffregen/XPT2046_Touchscreen@^1.4` | `https://github.com/PaulStoffregen/XPT2046_Touchscreen.git` |

---

**Bottom line:** The apt version works perfectly fine for this project. No upgrade needed unless you want the latest features!
