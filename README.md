# 🎵 Jam Wysteria - ESP32 Internet Radio Boombox

<p align="center">
  <img src="logo/jamwarehouse_logo_trans.png" alt="Jam Wysteria Logo" width="300"/>
</p>

A modern, touch-enabled internet radio streaming application for the ESP32-2432S028R (Cheap Yellow Display) with integrated web interface for easy station management.

## 📋 Table of Contents

- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Hardware Assembly](#hardware-assembly)
- [Installation](#installation)
- [Usage](#usage)
- [Web Interface](#web-interface)
- [Station Management](#station-management)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## ✨ Features

### Core Features
- **📡 Internet Radio Streaming**: Support for direct streams, m3u8 playlists, and various streaming formats
- **🎨 Modern Touch UI**: Intuitive 2.8" touchscreen interface with smooth navigation
- **📁 Hierarchical Organization**: Create folders within folders to organize your stations
- **⌨️ On-Screen Keyboard**: Full QWERTY keyboard with special characters for on-device configuration
- **🌐 WiFi Setup Wizard**: Easy first-time setup with network scanning and password entry
- **🎵 Rich Metadata Display**: Shows artist, title, album art, and stream information
- **🔊 Volume Control**: Easy volume adjustment with on-screen controls
- **⚙️ Settings Menu**: Change WiFi networks, adjust settings, and manage the device
- **💾 MicroSD Storage**: Store station logos, icons, and configuration on SD card

### Web Interface Features
- **🖥️ Browser-Based Management**: Configure stations from any device on your network
- **➕ Bulk Import**: Import multiple stations at once via CSV or JSON
- **📊 Visual Station Manager**: See your entire folder structure at a glance
- **🔄 Real-Time Sync**: Changes sync immediately to the device
- **📱 Mobile Responsive**: Works on phones, tablets, and desktops

### Player Features
- **⏯️ Playback Controls**: Play, pause, skip (for playlist streams)
- **📻 Live Stream Support**: Optimized for continuous streaming
- **🎚️ Equalizer**: Audio enhancement options
- **🔁 Auto-Reconnect**: Automatically reconnects to streams if connection drops

## 🛠️ Hardware Requirements

### Required Components

1. **ESP32-2432S028R Development Board** ("Cheap Yellow Display")
   - 2.8" 320x240 TFT touchscreen (ILI9341)
   - Built-in ESP32-WROOM-32 module
   - MicroSD card slot
   - RGB LED
   - USB-C programming port
   - [Product Link](https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG)

2. **MAX98357A I2S Audio Amplifier**
   - Class D 3W amplifier
   - I2S digital audio input
   - Mono or stereo configuration
   - [Option 1](https://www.amazon.com/MTDELE-MAX98357-Amplifier-Filterless-Compatible/dp/B0F3WYQWVG)
   - [Option 2](https://www.amazon.com/MAX98357-MAX98357A-Amplifier-Interface-Raspberry/dp/B0DPJRLMDJ/)

3. **Speaker(s)**
   - 4-8Ω impedance
   - 3W or higher power rating
   - Recommended: 2x 4Ω 5W speakers for stereo

4. **MicroSD Card**
   - Class 10 or higher
   - 1GB or larger (4GB recommended)
   - FAT32 formatted

5. **Power Supply**
   - 5V 2A USB-C power supply (for ESP32)
   - Note: MAX98357A is powered from ESP32 5V rail

### Optional Components
- 3D printed enclosure
- Battery pack for portable operation (5V USB power bank)
- Additional buttons for hardware controls
- LED strip for visual feedback

## 💻 Software Requirements

### Development Tools
- **Arduino IDE** 2.0 or higher (or PlatformIO)
- **ESP32 Board Support** (install via Board Manager)
- **USB-to-Serial Drivers** (CP210x or CH340, depending on your board)

### Required Arduino Libraries
```
- TFT_eSPI (v2.5.0+) - Display driver
- Audio (ESP32-audioI2S by schreibfaul1) - Audio streaming
- ArduinoJson (v6.21.0+) - JSON parsing
- ESPAsyncWebServer - Web server
- AsyncTCP - Async TCP library
- WiFiManager - WiFi configuration
- SD - SD card support
```

Install via Arduino Library Manager or see [Installation](#installation) for details.

## 🔌 Hardware Assembly

### Wiring Diagram

```
ESP32-2432S028R → MAX98357A Audio Amplifier
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

ESP32 Pin    →    MAX98357A Pin    Function
─────────────────────────────────────────────
GPIO 25      →    DIN (Data In)    I2S Data
GPIO 26      →    BCLK             I2S Clock
GPIO 27      →    LRC              I2S Word Select
5V           →    VIN              Power (+5V)
GND          →    GND              Ground
                  SD               Shutdown (connect to VIN for always-on)

MAX98357A → Speaker
━━━━━━━━━━━━━━━━━━━━━
+            →    Speaker +
-            →    Speaker -
```

### Assembly Steps

1. **Prepare the ESP32 Board**
   - Insert formatted MicroSD card into the slot
   - Ensure the display protector (if included) is properly installed

2. **Connect the Audio Amplifier**
   - Solder wires to the ESP32 breakout pins (GPIO 25, 26, 27)
   - Connect 5V and GND from the ESP32 to the MAX98357A
   - For stereo: use two MAX98357A modules (see stereo wiring below)

3. **Connect Speakers**
   - Connect speaker wires to the MAX98357A output terminals
   - Observe polarity (+ and -)

4. **Power Connection**
   - Use a quality 5V 2A USB-C power supply
   - For portable use, connect a USB power bank

### Stereo Configuration (Optional)

For stereo output, use two MAX98357A modules:

```
Left Channel:
- GPIO 25 → DIN (Left Amp)
- GPIO 26 → BCLK (Left Amp)
- GPIO 27 → LRC (Left Amp)
- Tie SD pin to GND

Right Channel:
- GPIO 25 → DIN (Right Amp)
- GPIO 26 → BCLK (Right Amp)
- GPIO 27 → LRC (Right Amp)
- Tie SD pin to VIN
```

## 📥 Installation

### Step 1: Install Arduino IDE and ESP32 Support

1. Download and install [Arduino IDE 2.0+](https://www.arduino.cc/en/software)

2. Add ESP32 board support:
   - Open Arduino IDE
   - Go to **File → Preferences**
   - Add this URL to "Additional Board Manager URLs":
     ```
     https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
     ```
   - Go to **Tools → Board → Board Manager**
   - Search for "esp32" and install "esp32 by Espressif Systems"

### Step 2: Install Required Libraries

Open Arduino IDE and go to **Tools → Manage Libraries**, then install:

1. **TFT_eSPI** by Bodmer
2. **ESP32-audioI2S** by schreibfaul1
3. **ArduinoJson** by Benoit Blanchon (v6.x)
4. **ESPAsyncWebServer** by lacamera
5. **AsyncTCP** by dvarrel

### Step 3: Configure TFT_eSPI for CYD

The Cheap Yellow Display requires specific configuration:

1. Navigate to your Arduino libraries folder:
   - Windows: `Documents\Arduino\libraries\TFT_eSPI`
   - macOS: `~/Documents/Arduino/libraries/TFT_eSPI`
   - Linux: `~/Arduino/libraries/TFT_eSPI`

2. Open `User_Setup_Select.h`

3. Comment out the default setup and uncomment:
   ```cpp
   #include <User_Setups/Setup42_ILI9341_ESP32.h>
   ```

4. Or use the custom setup file provided in `firmware/tft_setup/`

### Step 4: Prepare the MicroSD Card

1. Format the MicroSD card as FAT32
2. Create the following folder structure:
   ```
   /logos/          (for station logos)
   /config/         (for configuration files)
   /icons/          (for custom icons)
   ```
3. Copy default assets from `sd_card_files/` to the root of the SD card

### Step 5: Upload the Firmware

1. Open `firmware/JamWysteria/JamWysteria.ino` in Arduino IDE

2. Select board settings:
   - **Board**: "ESP32 Dev Module"
   - **Flash Size**: "4MB"
   - **Partition Scheme**: "Default 4MB with spiffs"
   - **Port**: Select your COM/USB port

3. Click **Upload** (or press Ctrl+U)

4. Wait for compilation and upload (takes 2-3 minutes)

5. Open Serial Monitor (115200 baud) to see debug output

### Step 6: First Boot Setup

1. Device will boot into WiFi setup mode
2. Follow the on-screen instructions to select and connect to your WiFi network
3. Once connected, the device IP address will be displayed
4. You can now add stations via the touchscreen or web interface

## 🎯 Usage

### First-Time WiFi Setup

1. **Power on the device** - You'll see the Jam Wysteria splash screen
2. **WiFi Setup Wizard** - Tap anywhere to begin
3. **Select Network** - Scroll through available networks and tap to select
4. **Enter Password** - Use the on-screen keyboard to type your WiFi password
5. **Connect** - Tap "Connect" button
6. **Success** - Device will show "Connected!" and display the IP address

### Adding Stations via Touchscreen

1. **Tap "Add" button** on the home screen
2. **Choose type**: "Folder" or "Station"

#### Adding a Folder:
- Enter folder name using on-screen keyboard
- Optionally select a custom icon
- Tap "Save"

#### Adding a Station:
- **Stream Name**: Enter display name (e.g., "Jazz FM")
- **Stream URL**: Enter the stream URL (starts with https://)
- **Stream Icon**: Optional - select a logo or use default
- **Select Location**: Choose root or a folder to save the station in
- Tap "Save"

### Playing a Station

1. **Navigate** - Tap folders to browse your collection
2. **Select Station** - Tap a station to start playing
3. **Player Screen** - Shows:
   - Station logo
   - Currently playing: Artist - Title
   - Playback controls
   - Volume slider
   - Back button

### Settings Menu

Access via the ⚙️ gear icon:
- **WiFi Settings**: Change network or reconnect
- **Display Settings**: Brightness, sleep timeout
- **Audio Settings**: Default volume, equalizer
- **About**: Version info, IP address, storage info

## 🌐 Web Interface

### Accessing the Web Interface

1. Find your device's IP address:
   - Shown on the display after WiFi connection
   - Check your router's DHCP client list
   - Look in Serial Monitor output

2. Open a web browser and navigate to:
   ```
   http://[DEVICE-IP-ADDRESS]
   ```
   Example: `http://192.168.1.100`

### Web Interface Features

#### Station Manager
- View all stations and folders in a tree structure
- Drag and drop to reorganize
- Edit station details
- Delete stations or folders
- Upload custom logos

#### Bulk Import

**CSV Format:**
```csv
type,name,url,icon,parent_folder
station,Jazz FM,https://example.com/jazz.m3u8,jazz_logo.png,/Music/Jazz
station,Rock Radio,https://example.com/rock,rock_logo.png,/Music/Rock
folder,News,,,/
```

**JSON Format:**
```json
[
  {
    "type": "folder",
    "name": "Music",
    "parent": "/"
  },
  {
    "type": "station",
    "name": "Jazz FM",
    "url": "https://example.com/jazz.m3u8",
    "icon": "jazz_logo.png",
    "parent": "/Music"
  }
]
```

#### Admin Panel
- WiFi network scanner and configuration
- System information
- Restart device
- Factory reset
- Backup/restore configuration

## 📻 Station Management

### Supported Stream Formats

Jam Wysteria supports various streaming formats:
- **Direct MP3/AAC streams** (e.g., `http://stream.example.com:8000`)
- **M3U/M3U8 playlists**
- **PLS playlists**
- **HLS streams**
- **Icecast/Shoutcast streams**

### Finding Stream URLs

Popular sources for internet radio streams:
- [Radio Browser](https://www.radio-browser.info/)
- [TuneIn](https://tunein.com/)
- [Shoutcast Directory](https://directory.shoutcast.com/)
- Individual station websites (look for "Listen Live" or "Stream URL")

### Tips for Best Results

1. **Test streams first**: Use VLC or another player to verify the URL works
2. **Use direct stream URLs**: These are more reliable than playlist URLs
3. **Optimize logos**: Keep logos under 100KB for faster loading
4. **Organize with folders**: Create a logical hierarchy (Genre → Country → Station)
5. **Backup your configuration**: Use the web interface to export your station list

## 🔧 Troubleshooting

### Display Issues

**Problem**: Display is blank or white
- **Solution**: Check TFT_eSPI configuration, verify pin assignments
- **Solution**: Ensure board is selected correctly in Arduino IDE

**Problem**: Touch not responding
- **Solution**: Calibrate touch in settings menu
- **Solution**: Check if screen protector is too thick

### Audio Issues

**Problem**: No audio output
- **Check**: Wiring connections (GPIO 25, 26, 27)
- **Check**: Speaker connections and polarity
- **Check**: Volume level (not muted)
- **Check**: Stream URL is valid and accessible

**Problem**: Distorted or choppy audio
- **Solution**: Increase buffer size in `config.h`
- **Solution**: Use a better quality SD card
- **Solution**: Check WiFi signal strength
- **Solution**: Reduce stream bitrate or use different stream

**Problem**: Audio cuts out
- **Solution**: Improve WiFi signal (move closer to router)
- **Solution**: Check power supply (needs 2A minimum)
- **Solution**: Reduce simultaneous web interface usage

### WiFi Issues

**Problem**: Can't connect to WiFi
- **Solution**: Verify password is correct
- **Solution**: Check if network is 2.4GHz (ESP32 doesn't support 5GHz)
- **Solution**: Move closer to router
- **Solution**: Try factory reset and reconfigure

**Problem**: Connection drops frequently
- **Solution**: Improve WiFi signal strength
- **Solution**: Set a static IP in router settings
- **Solution**: Disable power saving in settings

### SD Card Issues

**Problem**: SD card not detected
- **Solution**: Ensure card is FAT32 formatted
- **Solution**: Try a different SD card (Class 10 recommended)
- **Solution**: Reinsert card firmly

**Problem**: Logos not displaying
- **Solution**: Check file paths are correct
- **Solution**: Ensure logos are in `/logos/` folder
- **Solution**: Convert images to JPG or PNG format
- **Solution**: Reduce image size (max 320x240px)

### Streaming Issues

**Problem**: "Stream Error" or won't play
- **Solution**: Verify URL in web browser or VLC first
- **Solution**: Some streams require specific user-agent headers
- **Solution**: Check if stream requires authentication
- **Solution**: Try a different stream source

**Problem**: Metadata not showing
- **Solution**: Not all streams provide metadata
- **Solution**: Check if stream is Icecast/Shoutcast compatible
- **Solution**: Verify stream header includes ICY metadata

## 🐛 Debug Mode

Enable debug output via Serial Monitor (115200 baud):

1. Uncomment `#define DEBUG_MODE` in `config.h`
2. Recompile and upload
3. Open Serial Monitor to see detailed logs

## 📝 Configuration Files

### config.json (SD Card)
```json
{
  "wifi_ssid": "YourNetwork",
  "wifi_password": "encrypted",
  "volume": 70,
  "brightness": 80,
  "last_station": "/Music/Jazz/Jazz FM"
}
```

### stations.json (SD Card)
```json
{
  "folders": [...],
  "stations": [...]
}
```

## 🤝 Contributing

Contributions are welcome! Please read our [Contributing Guidelines](CONTRIBUTING.md) before submitting PRs.

### Development Setup
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly on hardware
5. Submit a pull request

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- ESP32 Arduino Core team
- TFT_eSPI library by Bodmer
- ESP32-audioI2S library by schreibfaul1
- Random Nerd Tutorials for CYD documentation
- All contributors and testers

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/jam-wysteria/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/jam-wysteria/discussions)
- **Documentation**: [Wiki](https://github.com/yourusername/jam-wysteria/wiki)

---

<p align="center">
  Made with ❤️ for the maker community
</p>

<p align="center">
  <strong>⭐ Star this repo if you found it helpful!</strong>
</p>
