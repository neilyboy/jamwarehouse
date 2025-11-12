# Setup Checklist for Jam Wysteria

Use this checklist to ensure you don't miss any steps during your build.

## Pre-Build Phase

### ☐ Parts Procurement

**Required Components:**
- [ ] ESP32-2432S028R board (Cheap Yellow Display)
- [ ] MAX98357A I2S amplifier module
- [ ] Speaker (4-8Ω, 3W+ recommended)
- [ ] MicroSD card (Class 10, 4GB+ recommended)
- [ ] USB-C cable (data capable, not just charging)
- [ ] 5V 2A power adapter (2.4A or 3A recommended)

**Wiring Materials:**
- [ ] 22-24 AWG hookup wire (6 different colors recommended)
- [ ] Soldering iron and solder
- [ ] Wire strippers
- [ ] Heat shrink tubing or electrical tape
- [ ] Multimeter (for testing)

**Optional:**
- [ ] USB power bank (10,000mAh+ for portable use)
- [ ] Enclosure (3D printed, project box, or custom)
- [ ] Small heatsinks
- [ ] Decoupling capacitors (100µF, 470µF)
- [ ] Rubber feet for enclosure

### ☐ Software Prerequisites

- [ ] Computer with Windows, macOS, or Linux
- [ ] Arduino IDE 2.0+ installed OR PlatformIO installed
- [ ] USB drivers for ESP32 (CP210x or CH340)
- [ ] Text editor for configuration files
- [ ] Web browser for web interface testing

---

## Build Phase

### ☐ Hardware Assembly

**Step 1: Test Components**
- [ ] Connect ESP32 via USB-C
- [ ] Display powers on and shows backlight
- [ ] Check serial monitor shows boot messages

**Step 2: Prepare Wires**
- [ ] Cut 6 wires ~15cm each
- [ ] Strip 5mm from each end
- [ ] Tin wire ends with solder
- [ ] Label wires if needed

**Step 3: Solder Connections**
- [ ] GPIO 25 → MAX98357A DIN (Blue wire)
- [ ] GPIO 26 → MAX98357A BCLK (Green wire)
- [ ] GPIO 27 → MAX98357A LRC (Yellow wire)
- [ ] 5V → MAX98357A VIN (Red wire)
- [ ] GND → MAX98357A GND (Black wire)
- [ ] MAX98357A SD → VIN (bridge on amplifier)

**Step 4: Connect Speaker**
- [ ] Identify speaker polarity (+ and -)
- [ ] Connect SPEAKER+ to speaker positive
- [ ] Connect SPEAKER- to speaker negative
- [ ] Secure connections with heatshrink or tape

**Step 5: Verify Wiring**
- [ ] Use multimeter to check continuity
- [ ] Verify NO shorts between VCC and GND
- [ ] Visual inspection of all solder joints
- [ ] Ensure no loose wires

---

## Software Setup Phase

### ☐ Arduino IDE Configuration

**Install Arduino IDE:**
- [ ] Download Arduino IDE 2.0+ from arduino.cc
- [ ] Install Arduino IDE
- [ ] Launch and verify it works

**Add ESP32 Support:**
- [ ] Open File → Preferences
- [ ] Add ESP32 board manager URL to "Additional Board Manager URLs":
  ```
  https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
  ```
- [ ] Open Tools → Board → Board Manager
- [ ] Search for "esp32"
- [ ] Install "esp32 by Espressif Systems"

**Install Required Libraries:**
- [ ] Open Tools → Manage Libraries
- [ ] Search and install "TFT_eSPI" by Bodmer
- [ ] Search and install "ESP32-audioI2S" by schreibfaul1
- [ ] Search and install "ArduinoJson" by Benoit Blanchon
- [ ] Search and install "ESPAsyncWebServer" by lacamera
- [ ] Search and install "AsyncTCP" by dvarrel
- [ ] Search and install "XPT2046_Touchscreen" by Paul Stoffregen

### ☐ Configure TFT_eSPI Library

**Locate Library Folder:**
- [ ] Find Arduino libraries folder:
  - Windows: `Documents\Arduino\libraries\TFT_eSPI`
  - Mac: `~/Documents/Arduino/libraries/TFT_eSPI`
  - Linux: `~/Arduino/libraries/TFT_eSPI`

**Configure Display:**
- [ ] Copy `firmware/tft_setup/User_Setup.h` to `TFT_eSPI/User_Setup.h`
  - OR manually edit `User_Setup.h` with settings from provided file
- [ ] Verify these settings are correct:
  ```cpp
  #define ILI9341_DRIVER
  #define TFT_CS   15
  #define TFT_DC   2
  #define TFT_RST  -1
  #define TFT_MOSI 13
  #define TFT_MISO 12
  #define TFT_SCLK 14
  #define TFT_BL   27
  ```

### ☐ Prepare SD Card

**Format SD Card:**
- [ ] Insert SD card into computer
- [ ] Format as FAT32 (not exFAT or NTFS)
- [ ] Verify format completed successfully

**Create Folder Structure:**
- [ ] Create `/config/` folder
- [ ] Create `/logos/` folder
- [ ] Create `/icons/` folder

**Optional - Add Example Stations:**
- [ ] Copy `examples/stations-example.json` to `/config/stations.json`
- [ ] Safely eject SD card
- [ ] Insert into ESP32 SD card slot

---

## Firmware Upload Phase

### ☐ Load and Compile Firmware

**Open Project:**
- [ ] Clone or download Jam Wysteria repository
- [ ] Open `firmware/JamWysteria/JamWysteria.ino` in Arduino IDE

**Configure Board Settings:**
- [ ] Tools → Board → "ESP32 Dev Module"
- [ ] Tools → Upload Speed → "921600"
- [ ] Tools → Flash Size → "4MB (32Mb)"
- [ ] Tools → Partition Scheme → "Default 4MB with spiffs"
- [ ] Tools → Port → Select your COM/USB port
  - Windows: COMx (e.g., COM3)
  - Mac: /dev/cu.usbserial-xxxx
  - Linux: /dev/ttyUSB0

**Compile:**
- [ ] Click Verify/Compile button (✓)
- [ ] Wait for compilation to complete (2-3 minutes first time)
- [ ] Verify "Done compiling" message with no errors

**Upload:**
- [ ] Click Upload button (→)
- [ ] Wait for "Connecting..." message
- [ ] If stuck, press and hold BOOT button on ESP32, then press RESET
- [ ] Wait for upload to complete
- [ ] Verify "Done uploading" message

**Test Upload:**
- [ ] Open Tools → Serial Monitor
- [ ] Set baud rate to 115200
- [ ] Press RESET button on ESP32
- [ ] Verify boot messages appear
- [ ] Look for "Jam Wysteria" boot banner

---

## First Boot and Configuration

### ☐ Initial Power-On

**Display Test:**
- [ ] Power on device
- [ ] Verify display shows splash screen
- [ ] Check for "Jam Wysteria" logo and version

**WiFi Setup:**
- [ ] Device shows WiFi setup wizard
- [ ] Tap "Scan Networks"
- [ ] Wait for network list to appear

**Connect to Network:**
- [ ] Find your WiFi network in list
- [ ] Tap your network name
- [ ] Use on-screen keyboard to enter password
  - [ ] Switch to uppercase if needed
  - [ ] Switch to numbers/symbols if needed
- [ ] Tap "Connect" button
- [ ] Wait for connection confirmation

**Note IP Address:**
- [ ] Write down IP address shown on screen
  - Example: 192.168.1.100
- [ ] You'll need this for web interface

---

## Testing Phase

### ☐ Basic Function Tests

**Display Test:**
- [ ] Screen is clear and readable
- [ ] Colors look correct
- [ ] No dead pixels or artifacts
- [ ] Backlight brightness adequate

**Touch Test:**
- [ ] Tapping works accurately
- [ ] Can navigate through menus
- [ ] Keyboard input works
- [ ] No ghost touches or missed touches

**Audio Test:**
- [ ] Speaker makes sound (use test station)
- [ ] No excessive distortion
- [ ] Volume control works
- [ ] No hissing or buzzing when idle

**WiFi Test:**
- [ ] Device stays connected
- [ ] Can access web interface from browser
- [ ] Network signal strength adequate
- [ ] No frequent disconnections

### ☐ Add First Station

**Via Web Interface (Recommended):**
- [ ] Open browser on phone/computer
- [ ] Navigate to `http://[DEVICE-IP]`
- [ ] Click "Manage Stations"
- [ ] Click "+ Add Station"
- [ ] Enter station details:
  - Name: BBC World Service
  - URL: http://stream.live.vc.bbcmedia.co.uk/bbc_world_service
- [ ] Click "Save"
- [ ] Verify station appears in list

**Via Touch Screen (Alternative):**
- [ ] Tap + button on home screen
- [ ] Select "Add Station"
- [ ] Enter name using keyboard
- [ ] Enter URL using keyboard
- [ ] Save station

**Test Playback:**
- [ ] Tap station to play
- [ ] Verify audio starts within 10 seconds
- [ ] Check metadata displays (if available)
- [ ] Test volume controls
- [ ] Test play/pause button
- [ ] Test back button

### ☐ Optional Import Stations

**Import Example Stations:**
- [ ] Open web interface
- [ ] Go to "Manage Stations"
- [ ] Click "Import"
- [ ] Copy content from `examples/stations-example.json`
- [ ] Paste into import field
- [ ] Click "Import"
- [ ] Verify stations appear in list
- [ ] Test playing multiple stations

---

## Finalization Phase

### ☐ Enclosure (Optional)

**If Using Enclosure:**
- [ ] Mount ESP32 on standoffs or foam tape
- [ ] Secure amplifier module
- [ ] Mount speaker in appropriate location
- [ ] Cut holes for:
  - [ ] Display viewing area
  - [ ] USB-C port access
  - [ ] Speaker output (grill or holes)
  - [ ] Ventilation
- [ ] Route wires neatly
- [ ] Use zip ties for cable management
- [ ] Add rubber feet to bottom

### ☐ Customization

**Add Your Stations:**
- [ ] Research your favorite stations
- [ ] Find stream URLs (see README for sources)
- [ ] Add stations via web interface or touchscreen
- [ ] Organize into folders by genre/type

**Adjust Settings:**
- [ ] Set preferred volume level
- [ ] Adjust screen brightness
- [ ] Configure screen timeout (if desired)
- [ ] Set default startup behavior

**Optional Customization:**
- [ ] Upload custom station logos to SD card
- [ ] Create themed folders (Jazz, Rock, News, etc.)
- [ ] Add podcast streams
- [ ] Share your configuration (export JSON)

---

## Troubleshooting Checklist

If something doesn't work, check:

**Display Issues:**
- [ ] TFT_eSPI configuration is correct
- [ ] Board selection is "ESP32 Dev Module"
- [ ] SPI pins are correctly wired
- [ ] Backlight pin (GPIO 27) is connected

**Touch Issues:**
- [ ] XPT2046 library is installed
- [ ] TOUCH_CS pin (GPIO 33) is correct
- [ ] Touch calibration values may need adjustment
- [ ] Screen protector not too thick

**Audio Issues:**
- [ ] I2S pins connected (GPIO 25, 26, 27)
- [ ] Speaker polarity correct
- [ ] MAX98357A powered (5V, GND)
- [ ] Volume not at minimum
- [ ] Stream URL is valid and working

**WiFi Issues:**
- [ ] Using 2.4GHz network (not 5GHz)
- [ ] Password is correct
- [ ] Router allows new devices
- [ ] Signal strength adequate

**SD Card Issues:**
- [ ] Card formatted as FAT32
- [ ] Card is Class 10 or better
- [ ] Folder structure created
- [ ] Card fully inserted

**See detailed troubleshooting:** [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)

---

## Completion Checklist

Your build is complete when:

- [ ] Display shows stations and UI
- [ ] Touch screen responds to input
- [ ] Audio plays from speaker
- [ ] WiFi connects and stays connected
- [ ] Web interface accessible
- [ ] Can add and play stations
- [ ] Volume control works
- [ ] Settings menu accessible
- [ ] No error messages in serial monitor
- [ ] Device operates reliably for 1+ hour

---

## What's Next?

### Enjoy Your Radio!
- [ ] Add your favorite stations
- [ ] Explore international radio
- [ ] Discover new music genres
- [ ] Listen to news and podcasts

### Share Your Build
- [ ] Take photos of your project
- [ ] Post to GitHub Discussions
- [ ] Share on maker forums (Reddit r/esp32, Hackaday)
- [ ] Help others with their builds

### Contribute Back
- [ ] Report any bugs found
- [ ] Suggest improvements
- [ ] Share your station lists
- [ ] Contribute code enhancements
- [ ] Improve documentation

---

## Resources

- **Main Documentation**: [README.md](README.md)
- **Quick Start Guide**: [docs/QUICK_START.md](docs/QUICK_START.md)
- **Hardware Assembly**: [docs/HARDWARE_ASSEMBLY.md](docs/HARDWARE_ASSEMBLY.md)
- **Troubleshooting**: [docs/TROUBLESHOOTING.md](docs/TROUBLESHOOTING.md)
- **GitHub Issues**: Report bugs and requests
- **GitHub Discussions**: Ask questions and share

---

**Estimated Total Time**: 2-4 hours (including testing)  
**Difficulty Level**: Intermediate (basic soldering required)  
**Success Rate**: High with proper preparation

Happy building! 🎵📻
