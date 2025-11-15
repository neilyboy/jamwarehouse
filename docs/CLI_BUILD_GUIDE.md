# 🛠️ CLI Build Guide - PlatformIO Method

This guide provides an alternative, faster way to build and flash the Jam Wysteria firmware using the command line instead of Arduino IDE. This method is ideal for developers who prefer CLI workflows and faster build times.

## 📋 Table of Contents
- [Why Use CLI Build?](#why-use-cli-build)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Available Commands](#available-commands)
- [Detailed Usage](#detailed-usage)
- [Troubleshooting](#troubleshooting)
- [Advanced Usage](#advanced-usage)

## 🎯 Why Use CLI Build?

**Benefits over Arduino IDE:**
- ⚡ **Faster builds** - Incremental compilation (only rebuilds changed files)
- 🚀 **No IDE required** - Works from terminal
- 🔄 **Better dependency management** - Automatic library handling
- 🐧 **Perfect for Linux/Ubuntu** - Native command-line workflow
- 🤖 **CI/CD friendly** - Easy to automate
- 💾 **Smaller footprint** - No GUI overhead

**Note:** The Arduino IDE method is still fully supported and recommended for beginners!

## 📦 Prerequisites

### System Requirements
- **Ubuntu/Debian**: 20.04 or newer
- **Other Linux**: Any modern distribution
- **macOS**: 10.14 or newer
- **Windows**: WSL2 or native Windows 10/11
- **Python**: 3.6 or higher

### Hardware Requirements
- ESP32-2432S028R board connected via USB
- USB drivers installed (usually automatic on Linux)

## 🚀 Installation

### Step 1: Install PlatformIO Core

**Option A: Using pip (Recommended)**
```bash
pip install platformio
```

**Option B: Using installation script**
```bash
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/scripts/get-platformio.py | python3
```

**Verify installation:**
```bash
pio --version
```

You should see output like: `PlatformIO Core, version X.X.X`

### Step 2: Clone/Navigate to Project

```bash
cd /path/to/jamwarehouse
```

### Step 3: Install Dependencies

PlatformIO will automatically install all required libraries on first build, or you can pre-install them:

```bash
./build.sh deps
```

This installs:
- TFT_eSPI
- ESP32-audioI2S
- ArduinoJson
- ESPAsyncWebServer
- AsyncTCP
- XPT2046_Touchscreen
- Adafruit GFX Library

### Step 4: Configure Serial Port (Optional)

Edit `platformio.ini` if your ESP32 is on a different port:

```ini
upload_port = /dev/ttyUSB0  ; Linux
; upload_port = COM3        ; Windows
; upload_port = /dev/cu.usbserial-*  ; macOS
```

Or let PlatformIO auto-detect (recommended).

## ⚡ Quick Start

### Make Build Script Executable
```bash
chmod +x build.sh
```

### Build and Flash in One Command
```bash
./build.sh upload
```

That's it! The script will:
1. Install dependencies (first time only)
2. Compile the firmware
3. Upload to your ESP32
4. Show progress and success message

## 📝 Available Commands

### `./build.sh build`
Compile the firmware without uploading.

**When to use:**
- Check for compilation errors
- Generate firmware binary
- Before doing an upload

**Output:** `.pio/build/esp32dev/firmware.bin`

```bash
./build.sh build
```

### `./build.sh upload`
Build and flash firmware to ESP32.

**When to use:**
- Initial firmware flash
- Updating firmware after code changes
- Most common command

```bash
./build.sh upload
```

### `./build.sh monitor`
Open serial monitor to view debug output.

**Features:**
- 115200 baud rate
- ESP32 exception decoder
- Real-time log viewing

**Exit:** Press `Ctrl+C`

```bash
./build.sh monitor
```

### `./build.sh clean`
Remove all build files to force complete rebuild.

**When to use:**
- Build errors after library updates
- Switching between configs
- Free up disk space

```bash
./build.sh clean
```

### `./build.sh ports`
List all available serial ports.

**Helps with:**
- Finding your ESP32's port
- Troubleshooting connection issues

```bash
./build.sh ports
```

### `./build.sh deps`
Install or update all project dependencies.

**When to use:**
- First time setup
- After updating platformio.ini
- Fixing library issues

```bash
./build.sh deps
```

### `./build.sh help`
Display help message with all commands.

```bash
./build.sh help
```

## 🔧 Detailed Usage

### Complete Workflow Example

#### 1. Initial Setup
```bash
# Install PlatformIO
pip install platformio

# Navigate to project
cd /home/neil/CascadeProjects/jamwarehouse

# Make script executable
chmod +x build.sh

# Install dependencies
./build.sh deps
```

#### 2. First Build and Flash
```bash
# Connect ESP32 via USB

# List ports to verify connection
./build.sh ports

# Build and upload
./build.sh upload
```

#### 3. Monitor Output
```bash
# Open serial monitor in a new terminal
./build.sh monitor
```

#### 4. Making Changes
```bash
# Edit code with your favorite editor
nano firmware/JamWysteria/config.h

# Rebuild and flash (incremental, faster)
./build.sh upload

# Or just build to check for errors
./build.sh build
```

### Build + Monitor Workflow

**Terminal 1:**
```bash
./build.sh upload && ./build.sh monitor
```

This uploads the firmware and immediately opens the monitor.

**Terminal 2:**
Keep your editor open for live development.

## 🐛 Troubleshooting

### Port Permission Issues (Linux)

**Problem:** `Permission denied: '/dev/ttyUSB0'`

**Solution:**
```bash
# Add yourself to dialout group
sudo usermod -a -G dialout $USER

# Log out and back in, or:
newgrp dialout
```

### Auto-reset Not Working

**Problem:** ESP32 doesn't enter bootloader mode automatically.

**Solution:** Hold BOOT button while uploading, or:
```bash
# Use slower upload speed
pio run --target upload --upload-port /dev/ttyUSB0
```

### Build Errors After Updates

**Problem:** Compilation errors after git pull or library updates.

**Solution:**
```bash
# Clean and rebuild
./build.sh clean
./build.sh deps
./build.sh build
```

### Port Not Found

**Problem:** `Error: Please specify 'upload_port'`

**Solution:**
```bash
# List available ports
./build.sh ports

# Edit platformio.ini with correct port
nano platformio.ini
```

### Library Conflicts

**Problem:** Multiple versions of libraries installed.

**Solution:**
```bash
# Remove all packages and reinstall
rm -rf .pio
./build.sh deps
./build.sh build
```

### Out of Memory During Compilation

**Problem:** `cc1plus: out of memory allocating X bytes`

**Solution:**
```bash
# Reduce parallel build jobs
export PLATFORMIO_BUILD_FLAGS="-j2"
./build.sh build
```

## 🚀 Advanced Usage

### Custom Build Flags

Edit `platformio.ini` to add custom flags:

```ini
build_flags = 
    -D DEBUG_MODE=1
    -D CUSTOM_BUFFER_SIZE=8192
```

### OTA Updates

Configure Over-The-Air updates:

```ini
upload_protocol = espota
upload_port = 192.168.1.100  ; Your ESP32's IP
```

Then upload:
```bash
./build.sh upload
```

### Multiple Environments

Add different configurations in `platformio.ini`:

```ini
[env:esp32dev-debug]
build_flags = -D DEBUG_MODE=1

[env:esp32dev-release]
build_flags = -O2
```

Build specific environment:
```bash
pio run -e esp32dev-debug
```

### Direct PlatformIO Commands

You can also use PlatformIO directly for more control:

```bash
# Build specific environment
pio run -e esp32dev

# Upload with verbose output
pio run --target upload -v

# Upload to specific port
pio run --target upload --upload-port /dev/ttyUSB1

# Monitor with filters
pio device monitor --filter direct

# Full clean including dependencies
pio run --target fullclean

# Update platforms and libraries
pio pkg update
pio platform update
```

### Filesystem Upload (SPIFFS)

Upload data to the ESP32's filesystem:

```bash
# Create data folder if it doesn't exist
mkdir -p data

# Add files to data/

# Upload filesystem
pio run --target uploadfs
```

### Build for Different Boards

Modify `platformio.ini`:

```ini
[env:esp32-wroom]
board = esp32dev

[env:esp32-wrover]
board = esp-wrover-kit
```

Build for specific board:
```bash
pio run -e esp32-wrover
```

## 📊 Performance Comparison

### Build Times (Approximate)

**Arduino IDE:**
- Clean build: 120-180 seconds
- Incremental: 60-90 seconds

**PlatformIO CLI:**
- Clean build: 90-120 seconds
- Incremental: 10-30 seconds ⚡

**Why faster?**
- Better caching
- Parallel compilation
- Only rebuilds changed files
- Optimized dependency resolution

## 🔄 CI/CD Integration

### GitHub Actions Example

```yaml
name: Build Firmware

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v3
      - uses: actions/setup-python@v4
        with:
          python-version: '3.x'
      - name: Install PlatformIO
        run: pip install platformio
      - name: Build firmware
        run: |
          cd $GITHUB_WORKSPACE
          pio run
      - name: Upload artifact
        uses: actions/upload-artifact@v3
        with:
          name: firmware
          path: .pio/build/esp32dev/firmware.bin
```

## 🆚 Arduino IDE vs PlatformIO CLI

| Feature | Arduino IDE | PlatformIO CLI |
|---------|-------------|----------------|
| **Installation** | Large download (~500MB) | Minimal (~50MB) |
| **Build Speed** | Moderate | Fast ⚡ |
| **Incremental Builds** | Slower | Much faster |
| **Dependency Management** | Manual | Automatic |
| **CI/CD Ready** | No | Yes ✓ |
| **Beginner Friendly** | Yes ✓ | Moderate |
| **GUI Required** | Yes | No |
| **Terminal Workflow** | No | Yes ✓ |

## 🎓 Learning Resources

- [PlatformIO Documentation](https://docs.platformio.org/)
- [PlatformIO CLI Reference](https://docs.platformio.org/en/latest/core/userguide/index.html)
- [ESP32 Platform Docs](https://docs.platformio.org/en/latest/platforms/espressif32.html)

## 🤝 Need Help?

- **Arduino IDE method:** See main [README.md](../README.md)
- **PlatformIO issues:** Check [PlatformIO Community](https://community.platformio.org/)
- **Project issues:** [GitHub Issues](https://github.com/yourusername/jam-wysteria/issues)

---

**Pro Tip:** You can use both methods! Build with PlatformIO CLI for speed, but keep Arduino IDE around for library browsing and examples.
