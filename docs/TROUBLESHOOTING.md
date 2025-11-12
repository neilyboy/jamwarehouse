# Troubleshooting Guide

Comprehensive troubleshooting for common issues with Jam Wysteria.

## Table of Contents

1. [Display Issues](#display-issues)
2. [Touch Screen Issues](#touch-screen-issues)
3. [Audio Issues](#audio-issues)
4. [WiFi Issues](#wifi-issues)
5. [SD Card Issues](#sd-card-issues)
6. [Streaming Issues](#streaming-issues)
7. [Web Interface Issues](#web-interface-issues)
8. [Power Issues](#power-issues)
9. [Compilation Issues](#compilation-issues)
10. [General Stability Issues](#general-stability-issues)

---

## Display Issues

### Display is Completely Black

**Possible Causes:**
- TFT_eSPI library not configured correctly
- Wrong board selected in Arduino IDE
- Backlight not connected or configured
- Power supply issue

**Solutions:**

1. **Check TFT_eSPI Configuration:**
   ```cpp
   // In User_Setup.h, verify:
   #define ILI9341_DRIVER
   #define TFT_CS   15
   #define TFT_DC   2
   #define TFT_MOSI 13
   #define TFT_SCLK 14
   #define TFT_BL   27
   ```

2. **Verify Board Selection:**
   - Tools → Board → "ESP32 Dev Module"
   - Not "ESP32 Wrover Module" or other variants

3. **Check Backlight:**
   - Verify GPIO 27 is connected properly
   - Check PWM initialization in code
   - Manually test: `digitalWrite(27, HIGH);`

4. **Test with Simple Sketch:**
   ```cpp
   #include <TFT_eSPI.h>
   TFT_eSPI tft = TFT_eSPI();
   
   void setup() {
     tft.init();
     tft.fillScreen(TFT_RED);
     pinMode(27, OUTPUT);
     digitalWrite(27, HIGH);
   }
   void loop() {}
   ```

### Display Shows Garbled/Random Pixels

**Possible Causes:**
- SPI pins incorrectly defined
- Interference on SPI bus
- Power supply noise

**Solutions:**

1. **Verify Pin Connections:**
   - Check continuity of SPI wires
   - Ensure no crossed connections
   - Verify solder joints

2. **Add Decoupling Capacitors:**
   - 100µF capacitor across VCC/GND near display
   - 0.1µF ceramic capacitor close to ESP32

3. **Reduce SPI Speed:**
   ```cpp
   // In User_Setup.h, try:
   #define SPI_FREQUENCY  27000000  // Reduce from 40MHz
   ```

### Display Works But Touch Doesn't

See [Touch Screen Issues](#touch-screen-issues) section below.

### Colors Are Wrong

**Possible Causes:**
- Color order setting incorrect
- BGR vs RGB mode

**Solutions:**

1. **Try Inverting Colors:**
   ```cpp
   // In setup(), add:
   tft.invertDisplay(true);  // or false
   ```

2. **Check Color Order:**
   ```cpp
   // In User_Setup.h:
   #define TFT_RGB_ORDER TFT_BGR  // or TFT_RGB
   ```

### Display Flickers

**Possible Causes:**
- Power supply insufficient
- Backlight PWM frequency too low
- Ground loop issues

**Solutions:**

1. **Use Better Power Supply:**
   - Minimum 5V 2A
   - Quality brand (Anker, etc.)

2. **Adjust PWM Frequency:**
   ```cpp
   // In config.h:
   #define BACKLIGHT_FREQ 5000  // Try 10000 or 20000
   ```

3. **Add Capacitors:**
   - 470µF across power rails
   - Close to ESP32 and display

---

## Touch Screen Issues

### Touch Not Responding At All

**Possible Causes:**
- XPT2046 library not installed
- Wrong TOUCH_CS pin
- Touch controller not initialized

**Solutions:**

1. **Verify Library Installed:**
   - Library Manager → "XPT2046_Touchscreen"

2. **Check Pin Definition:**
   ```cpp
   // In config.h:
   #define TOUCH_CS 33  // Verify this is correct
   ```

3. **Test with Simple Touch Sketch:**
   ```cpp
   #include <XPT2046_Touchscreen.h>
   XPT2046_Touchscreen ts(33);
   
   void setup() {
     Serial.begin(115200);
     ts.begin();
   }
   
   void loop() {
     if (ts.touched()) {
       TS_Point p = ts.getPoint();
       Serial.printf("X: %d, Y: %d\n", p.x, p.y);
       delay(100);
     }
   }
   ```

### Touch Position is Inaccurate

**Possible Causes:**
- Calibration values incorrect
- Screen rotation not matching touch rotation

**Solutions:**

1. **Recalibrate Touch:**
   ```cpp
   // In config.h, adjust these values:
   #define TOUCH_MIN_X 200
   #define TOUCH_MAX_X 3800
   #define TOUCH_MIN_Y 200
   #define TOUCH_MAX_Y 3800
   ```

2. **Match Rotations:**
   ```cpp
   // Ensure both are set to same rotation:
   tft.setRotation(1);
   touchscreen.setRotation(1);
   ```

3. **Run Calibration Routine:**
   - Touch corners of screen
   - Note raw values from serial monitor
   - Update config.h with new min/max values

### Touch Too Sensitive or Erratic

**Possible Causes:**
- Noise on touch lines
- No pull-up resistor on IRQ line
- Interference from WiFi

**Solutions:**

1. **Enable Touch Debouncing:**
   ```cpp
   // In config.h:
   #define TOUCH_DEBOUNCE_MS 200  // Increase if too sensitive
   ```

2. **Add Capacitor to Touch CS:**
   - 0.1µF ceramic cap between TOUCH_CS and GND

3. **Disable WiFi During Touch:**
   ```cpp
   // In touch handling code:
   WiFi.mode(WIFI_PS_MIN_MODEM);  // Minimum power save
   ```

### Touch Works But Screen Protector Blocks It

**Solutions:**

1. **Remove Thick Screen Protectors:**
   - Capacitive touch works through thin films only
   - Remove protectors > 0.5mm

2. **Adjust Touch Threshold:**
   - Some touch controllers allow threshold adjustment
   - Check XPT2046 library documentation

---

## Audio Issues

### No Audio Output At All

**Possible Causes:**
- I2S pins not connected
- Speaker not connected
- MAX98357A not powered
- Volume set to 0

**Solutions:**

1. **Verify Wiring:**
   ```
   GPIO 25 → MAX98357A DIN
   GPIO 26 → MAX98357A BCLK
   GPIO 27 → MAX98357A LRC
   5V      → MAX98357A VIN
   GND     → MAX98357A GND
   ```

2. **Check MAX98357A LED:**
   - Should light up when audio is playing
   - If not lit, check power connection

3. **Test Speaker:**
   ```cpp
   // Test with tone generation:
   Audio audio;
   audio.setPinout(26, 27, 25);
   audio.connecttospeech("This is a test", "en");
   ```

4. **Check Volume:**
   ```cpp
   Serial.println(AudioPlayer.getVolume());
   AudioPlayer.setVolume(15);  // Mid volume
   ```

### Audio is Distorted or Crackling

**Possible Causes:**
- Poor quality power supply
- Speaker impedance too low
- Volume too high
- Buffer underrun

**Solutions:**

1. **Use Quality Power Supply:**
   - 5V 2A minimum
   - Clean power with low ripple

2. **Check Speaker Impedance:**
   - Should be 4-8Ω
   - Lower impedance draws more current

3. **Reduce Volume:**
   ```cpp
   AudioPlayer.setVolume(10);  // Try lower volume
   ```

4. **Adjust GAIN Setting:**
   ```
   MAX98357A GAIN → GND (9dB, cleaner)
   Instead of VIN (15dB, louder but may distort)
   ```

5. **Increase Buffer Size:**
   ```cpp
   // In config.h:
   #define AUDIO_BUFFER_SIZE 16384  // Increase from 8192
   ```

### Audio Cuts Out or Stutters

**Possible Causes:**
- WiFi connection issues
- SD card too slow
- Insufficient memory
- Stream buffering issues

**Solutions:**

1. **Check WiFi Signal:**
   ```cpp
   Serial.println(WiFi.RSSI());
   // Should be > -70 dBm for good streaming
   ```

2. **Use Faster SD Card:**
   - Class 10 or UHS-I minimum
   - Format as FAT32

3. **Reduce Buffer Size if Memory Low:**
   ```cpp
   // Check free heap:
   Serial.println(ESP.getFreeHeap());
   // Should have > 50KB free
   ```

4. **Try Different Stream:**
   - Some streams are more reliable than others
   - Lower bitrate streams (64-128 kbps) work better

### Audio Has Hiss or Noise

**Possible Causes:**
- Ground loop
- Power supply noise
- Interference from display

**Solutions:**

1. **Add Filtering Capacitors:**
   - 100µF across ESP32 power
   - 470µF across MAX98357A power

2. **Separate Grounds:**
   - Star ground topology
   - Digital GND and analog GND meet at one point

3. **Shield Audio Wires:**
   - Use shielded cable for I2S signals
   - Ground shield at one end only

4. **Move Away from Display:**
   - Physical separation reduces EMI

---

## WiFi Issues

### Cannot Connect to WiFi

**Possible Causes:**
- Wrong password
- 5GHz network (ESP32 only supports 2.4GHz)
- Router security settings
- Network overcrowded

**Solutions:**

1. **Verify Password:**
   - Check for typos
   - Case sensitive
   - Special characters

2. **Check Network Frequency:**
   - ESP32 only supports 2.4GHz
   - Disable 5GHz or create separate 2.4GHz network

3. **Check Router Settings:**
   - Disable AP isolation
   - Enable DHCP
   - Check MAC filtering

4. **Try Manual IP:**
   ```cpp
   IPAddress local_IP(192, 168, 1, 100);
   IPAddress gateway(192, 168, 1, 1);
   IPAddress subnet(255, 255, 255, 0);
   
   WiFi.config(local_IP, gateway, subnet);
   ```

### WiFi Connects But No Internet

**Possible Causes:**
- DNS not configured
- Firewall blocking
- Router issue

**Solutions:**

1. **Set DNS Manually:**
   ```cpp
   IPAddress primaryDNS(8, 8, 8, 8);  // Google DNS
   IPAddress secondaryDNS(1, 1, 1, 1);  // Cloudflare DNS
   
   WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
   ```

2. **Test with Simple HTTP Request:**
   ```cpp
   HTTPClient http;
   http.begin("http://www.google.com");
   int httpCode = http.GET();
   Serial.println(httpCode);  // Should be 200 or 301
   ```

### WiFi Disconnects Frequently

**Possible Causes:**
- Weak signal
- Power saving mode
- Router timeout
- Interference

**Solutions:**

1. **Disable Power Save:**
   ```cpp
   WiFi.setSleep(false);
   ```

2. **Increase Signal Strength:**
   - Move closer to router
   - Remove obstacles
   - Use WiFi extender

3. **Set Static IP:**
   - Prevents DHCP timeout issues

4. **Add Keepalive:**
   ```cpp
   // Send periodic ping to router
   WiFi.setAutoReconnect(true);
   ```

---

## SD Card Issues

### SD Card Not Detected

**Possible Causes:**
- SD card not inserted properly
- SD card not formatted as FAT32
- Bad SD card
- Connection issues

**Solutions:**

1. **Check Insertion:**
   - Push firmly until click
   - Verify card is fully seated

2. **Format as FAT32:**
   - Use official SD formatter tool
   - Not exFAT or NTFS

3. **Try Different Card:**
   - Some cards incompatible
   - Use quality brand (SanDisk, Samsung)

4. **Check Pins:**
   - SD_CS, SD_MOSI, SD_MISO, SD_SCLK
   - Verify no shorts or opens

### Files Cannot Be Read/Written

**Possible Causes:**
- File system corrupted
- Permissions issue
- Path too long
- Filename characters

**Solutions:**

1. **Format and Try Again:**
   - Back up data first
   - Full format (not quick)

2. **Use Short Paths:**
   ```cpp
   // Good:  /config/cfg.json
   // Bad:   /very/long/path/to/my/configuration/file.json
   ```

3. **Check File Names:**
   - No special characters: \ / : * ? " < > |
   - Use underscores instead of spaces

4. **Test with Simple Sketch:**
   ```cpp
   #include <SD.h>
   
   void setup() {
     Serial.begin(115200);
     if (!SD.begin(5)) {
       Serial.println("SD failed");
       return;
     }
     
     File file = SD.open("/test.txt", FILE_WRITE);
     if (file) {
       file.println("Test");
       file.close();
       Serial.println("Write OK");
     }
   }
   ```

---

## Streaming Issues

### "Stream Error" or Cannot Play Station

**Possible Causes:**
- Invalid URL
- Stream offline
- Unsupported format
- Geoblocking
- Network firewall

**Solutions:**

1. **Test URL in Browser/VLC:**
   - Open URL in VLC Media Player
   - If doesn't work there, URL is bad

2. **Try Different Stream:**
   - Some stations go offline
   - Try alternative URL for same station

3. **Check Format:**
   - Works: MP3, AAC
   - May not work: WMA, FLAC, OGG

4. **Use VPN if Geoblocked:**
   - Some streams restricted by region

### Stream Metadata Not Showing

**Possible Causes:**
- Stream doesn't provide metadata
- Icecast metadata parsing issue

**Solutions:**

1. **Check if Stream Has Metadata:**
   ```bash
   curl -H "Icy-MetaData: 1" [STREAM_URL] -v
   # Look for "icy-metaint" in headers
   ```

2. **Some Streams Don't Provide It:**
   - This is normal for some streams
   - Station name may still display

3. **Enable Metadata in Code:**
   ```cpp
   // In Audio library setup:
   audio.setMetadata(true);
   ```

---

## Web Interface Issues

### Cannot Access Web Interface

**Possible Causes:**
- Device not connected to WiFi
- Wrong IP address
- Port blocked
- Browser cache

**Solutions:**

1. **Verify IP Address:**
   - Check serial monitor for IP
   - Ping device: `ping [IP ADDRESS]`

2. **Check Port:**
   - Should be port 80
   - Try: `http://192.168.1.100:80`

3. **Clear Browser Cache:**
   - Ctrl+F5 to hard refresh
   - Or clear browser cache

4. **Try Different Browser:**
   - Chrome, Firefox, Edge, Safari

5. **Check Firewall:**
   - Disable temporarily to test
   - Allow port 80 traffic

### Web Interface Loads But Features Don't Work

**Possible Causes:**
- JavaScript errors
- API endpoint issues
- CORS problems

**Solutions:**

1. **Check Browser Console:**
   - F12 to open developer tools
   - Look for JavaScript errors

2. **Verify API Endpoints:**
   ```bash
   curl http://[IP]/api/stations
   # Should return JSON
   ```

3. **Test Simple API Call:**
   - Open: `http://[IP]/api/config`
   - Should show device info

---

## Power Issues

### Device Resets or Reboots Randomly

**Possible Causes:**
- Insufficient power supply
- Voltage drops during WiFi transmission
- Brown-out detector triggered

**Solutions:**

1. **Use Higher Current Supply:**
   - Minimum 2A, recommend 2.4A or 3A
   - Quality brand

2. **Add Large Capacitor:**
   - 470-1000µF electrolytic
   - Across ESP32 power rails

3. **Check for Shorts:**
   - Verify no solder bridges
   - Check for damaged components

### Device Gets Hot

**Possible Causes:**
- Normal operation (ESP32 can get warm)
- Short circuit
- Amplifier working hard

**Solutions:**

1. **Normal if Warm (< 60°C):**
   - ESP32 typically runs 40-50°C
   - MAX98357A gets warm when playing

2. **Add Heatsink if Too Hot:**
   - Small aluminum heatsink on ESP32
   - Ensure ventilation in enclosure

3. **Check for Shorts:**
   - Power off immediately if very hot (> 70°C)
   - Check all connections

---

## Compilation Issues

### "Library Not Found" Errors

**Solutions:**

1. **Install Missing Libraries:**
   - Tools → Manage Libraries
   - Search and install each required library

2. **Check Library Names:**
   - Exact names matter
   - "TFT_eSPI" not "TFT-eSPI"

3. **Restart Arduino IDE:**
   - Sometimes needed after installing libraries

### "ESP32 Board Not Found"

**Solutions:**

1. **Install ESP32 Board Support:**
   - File → Preferences → Additional Board Manager URLs
   - Add ESP32 JSON URL
   - Tools → Board Manager → Install ESP32

2. **Select Correct Board:**
   - Tools → Board → ESP32 Dev Module

### Compilation Takes Very Long or Fails

**Solutions:**

1. **Increase Memory:**
   - Close other applications
   - Restart computer

2. **Clean Build:**
   - Sketch → Delete Build Folder
   - Recompile

3. **Update Arduino IDE:**
   - Use latest version 2.x

---

## General Stability Issues

### Device Crashes or Freezes

**Possible Causes:**
- Memory leak
- Stack overflow
- Watchdog timeout

**Solutions:**

1. **Check Free Heap:**
   ```cpp
   Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
   // Should stay relatively stable
   ```

2. **Enable Watchdog:**
   ```cpp
   #include "esp_task_wdt.h"
   
   void setup() {
     esp_task_wdt_init(30, true);  // 30 second timeout
     esp_task_wdt_add(NULL);
   }
   
   void loop() {
     esp_task_wdt_reset();
     // Your code...
   }
   ```

3. **Look for Infinite Loops:**
   - Add debug prints
   - Check serial monitor

### Touchscreen Freezes

**Possible Causes:**
- Touch handler blocking
- Long operations in loop
- Interrupt conflicts

**Solutions:**

1. **Don't Block in Touch Handler:**
   ```cpp
   // Bad:
   void handleTouch() {
     delay(1000);  // Blocks!
   }
   
   // Good:
   void handleTouch() {
     // Set flag, handle in loop
   }
   ```

2. **Use Async Operations:**
   - Don't wait for network in touch handler
   - Use callbacks

---

## Getting Help

If you can't resolve your issue:

1. **Check Serial Monitor:**
   - Often shows error messages
   - Set baud rate to 115200

2. **Enable Debug Mode:**
   ```cpp
   // In config.h:
   #define DEBUG_MODE
   ```

3. **Collect Information:**
   - Hardware versions
   - Firmware version
   - Exact error messages
   - Steps to reproduce

4. **Create GitHub Issue:**
   - Use bug report template
   - Include all information above
   - Photos/videos helpful

5. **Ask in Discussions:**
   - Community may have solution
   - Search existing threads first

---

## Advanced Debugging

### Using Serial Monitor Effectively

```cpp
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);  // Enable ESP32 debug
}

void loop() {
  Serial.printf("[%lu] Status: %s\n", millis(), status);
}
```

### Memory Debugging

```cpp
void printMemoryInfo() {
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Minimum free heap: %d\n", ESP.getMinFreeHeap());
  Serial.printf("Max alloc heap: %d\n", ESP.getMaxAllocHeap());
}
```

### Network Debugging

```cpp
#include <HTTPClient.h>

void testNetwork() {
  HTTPClient http;
  http.begin("http://www.google.com");
  http.setConnectTimeout(5000);
  int code = http.GET();
  Serial.printf("HTTP: %d\n", code);
  http.end();
}
```

---

**Still having issues?** Open an issue on GitHub with:
- Complete error messages
- Serial monitor output
- Hardware setup photos
- Steps to reproduce

We're here to help! 🛠️
