#!/bin/bash
# Jam Wysteria - PlatformIO CLI Build Script
# This script provides a simple CLI interface for building and flashing the firmware

set -e  # Exit on error

# Colors for output
GREEN='\033[0;32m'
BLUE='\033[0;34m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Project paths
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
FIRMWARE_SRC="$PROJECT_ROOT/firmware/JamWysteria"

echo -e "${BLUE}╔═══════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║   Jam Wysteria - Build & Flash Tool      ║${NC}"
echo -e "${BLUE}╚═══════════════════════════════════════════╝${NC}"
echo

# Check if PlatformIO is installed
if ! command -v pio &> /dev/null; then
    echo -e "${RED}❌ PlatformIO CLI is not installed!${NC}"
    echo -e "${YELLOW}Please install it first:${NC}"
    echo "  curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/scripts/get-platformio.py | python3"
    echo "  or: pip install platformio"
    echo
    exit 1
fi

echo -e "${GREEN}✓${NC} PlatformIO CLI detected"
echo

# Function to show help
show_help() {
    echo "Usage: ./build.sh [OPTION]"
    echo
    echo "Options:"
    echo "  build       - Compile the firmware (doesn't upload)"
    echo "  upload      - Build and upload firmware to ESP32"
    echo "  monitor     - Open serial monitor"
    echo "  clean       - Clean build files"
    echo "  ports       - List available serial ports"
    echo "  deps        - Install/update all dependencies"
    echo "  help        - Show this help message"
    echo
    echo "Examples:"
    echo "  ./build.sh build          # Just compile"
    echo "  ./build.sh upload         # Build and flash to ESP32"
    echo "  ./build.sh monitor        # View serial output"
    echo
}

# Function to list ports
list_ports() {
    echo -e "${BLUE}Available serial ports:${NC}"
    pio device list
}

# Function to build
do_build() {
    echo -e "${BLUE}🔨 Building firmware...${NC}"
    cd "$PROJECT_ROOT"
    pio run
    echo
    echo -e "${GREEN}✓ Build complete!${NC}"
    echo -e "Firmware binary: .pio/build/esp32dev/firmware.bin"
}

# Function to upload
do_upload() {
    echo -e "${BLUE}📤 Building and uploading firmware...${NC}"
    cd "$PROJECT_ROOT"
    
    # Check if port is specified in platformio.ini or auto-detect
    echo -e "${YELLOW}Note: Make sure your ESP32 is connected via USB${NC}"
    echo
    
    pio run --target upload
    echo
    echo -e "${GREEN}✓ Upload complete!${NC}"
    echo -e "${YELLOW}💡 Tip: Use './build.sh monitor' to view serial output${NC}"
}

# Function to monitor
do_monitor() {
    echo -e "${BLUE}📟 Opening serial monitor (115200 baud)${NC}"
    echo -e "${YELLOW}Press Ctrl+C to exit${NC}"
    echo
    cd "$PROJECT_ROOT"
    pio device monitor --baud 115200 --filter esp32_exception_decoder
}

# Function to clean
do_clean() {
    echo -e "${BLUE}🧹 Cleaning build files...${NC}"
    cd "$PROJECT_ROOT"
    pio run --target clean
    echo -e "${GREEN}✓ Clean complete!${NC}"
}

# Function to install dependencies
do_deps() {
    echo -e "${BLUE}📦 Installing/updating dependencies...${NC}"
    cd "$PROJECT_ROOT"
    pio pkg install
    pio pkg update
    echo -e "${GREEN}✓ Dependencies installed!${NC}"
}

# Parse command line arguments
case "${1:-help}" in
    build)
        do_build
        ;;
    upload)
        do_upload
        ;;
    monitor)
        do_monitor
        ;;
    clean)
        do_clean
        ;;
    ports)
        list_ports
        ;;
    deps)
        do_deps
        ;;
    help|--help|-h)
        show_help
        ;;
    *)
        echo -e "${RED}Unknown option: $1${NC}"
        echo
        show_help
        exit 1
        ;;
esac
