# ⚡ Quick Start - CLI Build Method

Ultra-quick reference for building Jam Wysteria using PlatformIO CLI.

## 🚀 One-Time Setup

```bash
# 1. Install PlatformIO
pip install platformio

# 2. Navigate to project
cd /home/neil/CascadeProjects/jamwarehouse

# 3. Make build script executable (already done)
chmod +x build.sh
```

## 🔨 Common Commands

```bash
# Build and flash to ESP32 (most common)
./build.sh upload

# View serial output
./build.sh monitor

# Just compile (check for errors)
./build.sh build

# Clean build files
./build.sh clean

# Install/update libraries
./build.sh deps

# List available serial ports
./build.sh ports

# Show all commands
./build.sh help
```

## 📖 Full Documentation

See **[docs/CLI_BUILD_GUIDE.md](docs/CLI_BUILD_GUIDE.md)** for complete instructions.

## 🆚 Why CLI vs Arduino IDE?

✅ **Much faster** - 10-30s incremental builds vs 60-90s  
✅ **No IDE needed** - pure command line  
✅ **Perfect for Ubuntu/Linux** - native workflow  
✅ **Automatic dependencies** - no manual library installation  

## 🔄 Typical Workflow

```bash
# 1. Make code changes with your editor
nano firmware/JamWysteria/config.h

# 2. Build and flash
./build.sh upload

# 3. Monitor output (optional)
./build.sh monitor
```

## 🐛 Troubleshooting

**Permission denied on port:**
```bash
sudo usermod -a -G dialout $USER
newgrp dialout
```

**Can't find port:**
```bash
./build.sh ports
# Edit platformio.ini with correct port
```

**Build errors:**
```bash
./build.sh clean
./build.sh deps
./build.sh build
```

---

**Need more help?** Check [docs/CLI_BUILD_GUIDE.md](docs/CLI_BUILD_GUIDE.md)
