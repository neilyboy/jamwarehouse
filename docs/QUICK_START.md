# Quick Start Guide

Get your Jam Wysteria Internet Radio up and running in 30 minutes!

## Prerequisites Checklist

Before you begin, make sure you have:

- [ ] ESP32-2432S028R board (Cheap Yellow Display)
- [ ] MAX98357A I2S amplifier module
- [ ] Speaker (4-8Ω, 3W+)
- [ ] MicroSD card (Class 10, FAT32 formatted)
- [ ] USB-C cable and 5V 2A power adapter
- [ ] Computer with Arduino IDE or PlatformIO installed
- [ ] Soldering iron and solder
- [ ] Hookup wire (22-24 AWG)

## Step 1: Hardware Assembly (15 minutes)

### Quick Wiring Guide

Connect the MAX98357A to ESP32 as follows:

| ESP32 Pin | MAX98357A Pin | Wire Color (suggested) |
|-----------|---------------|------------------------|
| GPIO 22   | LRC          | Yellow                 |
| GPIO 25   | DIN          | Blue                   |
| GPIO 26   | BCLK         | Green                  |
| 5V        | VIN          | Red                    |
| GND       | GND          | Black                  |
| -         | SD → VIN     | (Bridge on amplifier)  |

**Important**: GPIO 27 is used for display backlight. Use GPIO 22 for I2S_LRC.

**Speaker Connection:**
- MAX98357A SPEAKER+ → Speaker (+) Red
- MAX98357A SPEAKER- → Speaker (-) Black

**Detailed instructions:** See [Hardware Assembly Guide](HARDWARE_ASSEMBLY.md)

## Step 2: Prepare the SD Card (5 minutes)

1. **Format the SD card as FAT32**
   - Windows: Right-click drive → Format → FAT32
   - Mac: Disk Utility → Erase → MS-DOS (FAT)
   - Linux: `sudo mkfs.vfat -F 32 /dev/sdX1`

2. **Create folder structure:**
   ```
   /config/
   /logos/
   /icons/
   ```

3. **Optional:** Copy example stations
   - Copy `examples/stations-example.json` to `/config/stations.json` on SD card

4. **Insert SD card** into ESP32 board slot

## Step 3: Install Arduino IDE and Libraries (10 minutes)

### Arduino IDE Setup

1. **Download Arduino IDE 2.0+** from [arduino.cc](https://www.arduino.cc/en/software)

2. **Add ESP32 Board Support:**
   - Open Arduino IDE
   - Go to: File → Preferences
   - Add to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to: Tools → Board → Board Manager
   - Search "esp32" and install "esp32 by Espressif Systems"

3. **Install Required Libraries:**

   Open: Tools → Manage Libraries, then search and install:

   - `TFT_eSPI` by Bodmer (v2.5.0+)
   - `ESP32-audioI2S` by schreibfaul1 (v3.0.0+)
   - `ArduinoJson` by Benoit Blanchon (v6.21.0+)
   - `ESPAsyncWebServer` by lacamera
   - `AsyncTCP` by dvarrel  
   - `XPT2046_Touchscreen` by Paul Stoffregen

### Configure TFT_eSPI Library

**Important:** This step is required for the display to work!

1. Locate your Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\TFT_eSPI`
   - Mac: `~/Documents/Arduino/libraries/TFT_eSPI`
   - Linux: `~/Arduino/libraries/TFT_eSPI`

2. **Option A - Quick Setup:**
   - Copy `firmware/tft_setup/User_Setup.h` to `TFT_eSPI/User_Setup.h` (overwrite existing)

3. **Option B - Manual Setup:**
   - Open `TFT_eSPI/User_Setup.h`
   - Uncomment: `#define ILI9341_DRIVER`
   - Set these values:
     ```cpp
     #define TFT_CS   15
     #define TFT_DC   2
     #define TFT_RST  -1
     #define TFT_MOSI 13
     #define TFT_MISO 12
     #define TFT_SCLK 14
     #define TFT_BL   27
     ```

## Step 4: Upload Firmware (5 minutes)

1. **Open the project:**
   - Open `firmware/JamWysteria/JamWysteria.ino` in Arduino IDE

2. **Configure Board Settings:**
   - Tools → Board → "ESP32 Dev Module"
   - Tools → Upload Speed → "921600"
   - Tools → Flash Size → "4MB (32Mb)"
   - Tools → Partition Scheme → "Default 4MB with spiffs"
   - Tools → Port → Select your COM/USB port

3. **Compile and Upload:**
   - Click Upload button (→) or press Ctrl+U
   - Wait for compilation (2-3 minutes first time)
   - Wait for upload to complete

4. **Open Serial Monitor:**
   - Tools → Serial Monitor
   - Set baud rate to: 115200
   - Watch for boot messages

## Step 5: First Boot Setup (5 minutes)

1. **Power On:**
   - You should see the "Jam Wysteria" splash screen
   - Display should show WiFi setup wizard

2. **Connect to WiFi:**
   - Tap "Scan Networks"
   - Select your WiFi network from the list
   - Enter password using on-screen keyboard
   - Tap "Connect"

3. **Note the IP Address:**
   - After connection, device displays IP address
   - Write this down (e.g., 192.168.1.100)
   - You'll use this to access the web interface

## Step 6: Add Your First Station

### Option A: Using Web Interface (Easiest)

1. **Open web browser** on your phone/computer
2. Navigate to: `http://[DEVICE-IP-ADDRESS]`
3. Click "Manage Stations"
4. Click "+ Add Station"
5. Fill in:
   - **Name:** BBC World Service
   - **URL:** `http://stream.live.vc.bbcmedia.co.uk/bbc_world_service`
6. Click "Save"

### Option B: Using Touch Screen

1. Tap the **+** button on home screen
2. Select "Add Station"
3. Use on-screen keyboard to enter:
   - Station name
   - Stream URL
4. Save

### Option C: Import Example Stations

1. Open web interface
2. Click "Manage Stations" → "Import"
3. Copy content from `examples/stations-example.json`
4. Paste and click "Import"
5. Enjoy 8+ pre-configured stations!

## Step 7: Play Your First Station

1. **On Touch Screen:**
   - Tap a station from the home screen
   - Player screen appears
   - Audio should start playing
   - Adjust volume using on-screen slider

2. **Controls:**
   - ⏸ = Pause/Play
   - ⏹ = Stop
   - ← = Back to station list
   - 🔊 = Volume control

## Troubleshooting

### Display Not Working

- Check TFT_eSPI configuration
- Verify SPI pin connections
- Try re-uploading firmware

### No Audio

- Check I2S wiring (GPIO 25, 26, 27)
- Verify speaker connection
- Check volume level
- Test stream URL in web browser

### WiFi Not Connecting

- Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- Check password is correct
- Move closer to router
- Check router allows new devices

### Touch Not Responding

- Check touch calibration in `config.h`
- Verify TOUCH_CS pin (GPIO 33)
- Remove thick screen protectors

## Next Steps

### Customize Your Radio

1. **Add More Stations:**
   - Find streams at [radio-browser.info](https://www.radio-browser.info/)
   - Add your favorite stations
   - Organize into folders (Music, News, Sports, etc.)

2. **Upload Station Logos:**
   - Save logo images (JPG/PNG, max 100KB) to SD card `/logos/` folder
   - Reference in station settings

3. **Adjust Settings:**
   - Volume default level
   - Screen brightness
   - Screen timeout

### Build an Enclosure

- 3D print custom case (STL files coming soon)
- Use project box and drill holes
- Build wooden speaker box
- Get creative!

### Share Your Build

- Post photos to GitHub Discussions
- Share on maker forums (Reddit r/esp32, Hackaday, etc.)
- Help others with their builds

## Finding Radio Streams

### Sources for Stream URLs:

1. **Radio Browser API:**
   - Website: [radio-browser.info](https://www.radio-browser.info/)
   - Huge database of internet radio stations
   - Filter by country, genre, language

2. **Station Websites:**
   - Visit your favorite station's website
   - Look for "Listen Live" or "Stream"
   - Right-click and copy stream URL

3. **TuneIn:**
   - Browse [tunein.com](https://tunein.com/)
   - Find station, inspect network traffic for stream URL

4. **Shoutcast Directory:**
   - Browse [directory.shoutcast.com](https://directory.shoutcast.com/)
   - Thousands of stations listed

### Stream URL Formats:

- **Direct Stream:** `http://station.com:8000/live.mp3`
- **M3U Playlist:** `http://station.com/playlist.m3u8`
- **PLS Playlist:** `http://station.com/station.pls`

**Tip:** Direct MP3/AAC streams work best!

## Support Resources

- **Documentation:** [README.md](../README.md)
- **Hardware Guide:** [HARDWARE_ASSEMBLY.md](HARDWARE_ASSEMBLY.md)
- **GitHub Issues:** Report bugs and request features
- **Discussions:** Ask questions, share builds

## What's Next?

You now have a working internet radio! Enjoy exploring thousands of stations from around the world.

**Ideas to Try:**
- Create themed folders (Jazz, Rock, News, Podcasts)
- Set up favorite morning news stations
- Discover international stations
- Add podcast streams
- Build multiple units for different rooms

Happy listening! 🎵📻

---

**Estimated Total Time:** 30-40 minutes

**Difficulty:** Beginner-Intermediate (basic soldering required)

**Cost:** ~$40-60 USD for all components
