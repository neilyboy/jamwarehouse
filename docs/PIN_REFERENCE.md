# ESP32-2432S028R Pin Reference for Jam Wysteria

Complete pin mapping and availability reference for the Cheap Yellow Display (CYD) board.

## Quick Reference Table

| Pin | Function | Used By | Available? | Notes |
|-----|----------|---------|------------|-------|
| GPIO 0 | Boot Mode | System | ❌ No | Used for boot mode selection |
| GPIO 1 | UART TX | USB Serial | ❌ No | Serial debugging/programming |
| GPIO 2 | TFT DC | Display | ❌ No | Display data/command select |
| GPIO 3 | UART RX | USB Serial | ❌ No | Serial debugging/programming |
| GPIO 4 | RGB LED Red | Optional LED | ⚠️ Shared | Can use if not using RGB LED |
| GPIO 5 | SD CS | SD Card | ❌ No | SD card chip select |
| GPIO 12 | TFT MISO | Display | ❌ No | Display SPI MISO |
| GPIO 13 | TFT MOSI | Display | ❌ No | Display SPI MOSI |
| GPIO 14 | TFT SCLK | Display | ❌ No | Display SPI clock |
| GPIO 15 | TFT CS | Display | ❌ No | Display chip select |
| GPIO 16 | RGB LED Green | Optional LED | ⚠️ Shared | Can use if not using RGB LED |
| GPIO 17 | RGB LED Blue | Optional LED | ⚠️ Shared | Can use if not using RGB LED |
| GPIO 18 | SD SCLK | SD Card | ❌ No | SD card SPI clock |
| GPIO 19 | SD MISO | SD Card | ❌ No | SD card SPI MISO |
| GPIO 21 | I2C SDA | Expansion | ✅ Yes | Available on CN1 header |
| **GPIO 22** | **I2S LRC** | **Audio** | ✅ **Yes** | **Available on CN1 header** |
| GPIO 23 | SD MOSI | SD Card | ❌ No | SD card SPI MOSI |
| **GPIO 25** | **I2S DOUT** | **Audio** | ✅ **Yes** | **Available on expansion** |
| **GPIO 26** | **I2S BCLK** | **Audio** | ✅ **Yes** | **Available on expansion** |
| GPIO 27 | TFT Backlight | Display | ❌ No | **DO NOT USE for I2S!** |
| GPIO 32 | I2C SCL | Expansion | ✅ Yes | Available on CN1 header |
| GPIO 33 | Touch CS | Touch Screen | ❌ No | Touch chip select |
| GPIO 34 | Input Only | - | ✅ Yes | Input only, no pullup |
| GPIO 35 | Input Only | - | ✅ Yes | Input only, no pullup |
| GPIO 36 (VP) | Touch IRQ | Touch Screen | ❌ No | Touch interrupt |
| GPIO 39 (VN) | Input Only | - | ✅ Yes | Input only, no pullup |

## Pin Groups

### Display Pins (ILI9341) - **DO NOT MODIFY**
```
TFT_CS   = GPIO 15  (Chip Select)
TFT_DC   = GPIO 2   (Data/Command)
TFT_MOSI = GPIO 13  (SPI MOSI)
TFT_MISO = GPIO 12  (SPI MISO)
TFT_SCLK = GPIO 14  (SPI Clock)
TFT_LED  = GPIO 27  (Backlight PWM)
```

### Touch Pins (XPT2046) - **DO NOT MODIFY**
```
TOUCH_CS  = GPIO 33  (Chip Select)
TOUCH_IRQ = GPIO 36  (Interrupt)
```
*Touch shares SPI bus with display*

### SD Card Pins - **DO NOT MODIFY**
```
SD_CS   = GPIO 5   (Chip Select)
SD_MOSI = GPIO 23  (SPI MOSI)
SD_MISO = GPIO 19  (SPI MISO)
SD_SCLK = GPIO 18  (SPI Clock)
```

### Audio Pins (I2S to MAX98357A) - **CONFIGURED FOR PROJECT**
```
I2S_DOUT = GPIO 25  (Data Out - DIN on MAX98357A)
I2S_BCLK = GPIO 26  (Bit Clock)
I2S_LRC  = GPIO 22  (Word Select / Left-Right Clock)
```

### Optional/Available Pins
```
GPIO 21 = I2C SDA (Available on CN1 connector)
GPIO 32 = I2C SCL (Available on CN1 connector)
GPIO 4  = RGB LED Red (if not using LED)
GPIO 16 = RGB LED Green (if not using LED)
GPIO 17 = RGB LED Blue (if not using LED)
GPIO 34 = Analog Input only
GPIO 35 = Analog Input only
GPIO 39 = Analog Input only
```

## Physical Connector Locations

### CN1 (Expansion Header - Top Right of Board)
```
Pin 1:  5V
Pin 2:  GND
Pin 3:  GPIO 35
Pin 4:  GPIO 22  ← I2S_LRC connects here
Pin 5:  GPIO 21  (I2C SDA)
Pin 6:  GPIO 32  (I2C SCL)
```

### P3 (GPIO Header - Bottom of Board)
```
Available GPIO pins for expansion:
- GPIO 25  ← I2S_DOUT connects here
- GPIO 26  ← I2S_BCLK connects here
- GND
- 5V
- 3.3V
```

### Speaker Interface Connector
```
2-pin JST connector:
- Pin 1: Speaker +
- Pin 2: Speaker -
Note: This is NOT amplified. Use MAX98357A instead.
```

## Important Warnings

### ⚠️ GPIO 27 Conflict - CRITICAL
**DO NOT use GPIO 27 for I2S_LRC!**

- GPIO 27 controls the TFT backlight
- Using it for audio will cause display flickering
- Backlight may turn off during audio playback
- **Solution:** Use GPIO 22 for I2S_LRC instead

### ⚠️ Strapping Pins
Some pins have special boot functions:

- **GPIO 0**: Must be HIGH during boot (pulled up)
- **GPIO 2**: Must be LOW during boot (has internal pulldown)
- **GPIO 12**: Flash voltage selection (don't use)
- **GPIO 15**: Must be HIGH during boot (pulled up)

### ⚠️ Input-Only Pins
These pins cannot output signals:

- GPIO 34, 35, 36 (VP), 39 (VN)
- No internal pull-up/pull-down resistors
- Use only for analog reading or digital input

## Wiring Summary for Jam Wysteria

### Required Connections
```
ESP32-2432S028R          MAX98357A Amplifier
─────────────────        ───────────────────

GPIO 22 (CN1 Pin 4) ───→ LRC  (Word Select)
GPIO 25 (P3)        ───→ DIN  (I2S Data)
GPIO 26 (P3)        ───→ BCLK (Bit Clock)
5V                  ───→ VIN  (Power)
GND                 ───→ GND  (Ground)

MAX98357A                Speaker
─────────                ───────
SPEAKER+            ───→ Red (+)
SPEAKER-            ───→ Black (-)

Bridge on MAX98357A:
SD → VIN (always on, no shutdown)
```

### Where to Solder

1. **GPIO 22:** Solder to pin 4 on CN1 connector (top right of board)
2. **GPIO 25:** Solder to P3 header or available pad (bottom/side of board)
3. **GPIO 26:** Solder to P3 header or available pad (bottom/side of board)
4. **5V & GND:** Multiple locations available:
   - CN1 connector (pins 1 & 2)
   - P3 header
   - Speaker connector power pins

## Testing Pin Accessibility

### How to Verify Your Board

1. **Visual Inspection:**
   - Locate CN1 connector (usually labeled on board)
   - Find P3 or GPIO expansion header
   - Check for labeled pins

2. **Multimeter Test:**
   - Set multimeter to continuity mode
   - Touch one probe to ESP32 chip pin
   - Touch other probe to header/pad
   - Should beep if connected

3. **Test Sketch:**
```cpp
void setup() {
  Serial.begin(115200);
  
  // Test GPIO 22
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);
  delay(500);
  digitalWrite(22, LOW);
  Serial.println("GPIO 22 toggled");
  
  // Test GPIO 25
  pinMode(25, OUTPUT);
  digitalWrite(25, HIGH);
  delay(500);
  digitalWrite(25, LOW);
  Serial.println("GPIO 25 toggled");
  
  // Test GPIO 26
  pinMode(26, OUTPUT);
  digitalWrite(26, HIGH);
  delay(500);
  digitalWrite(26, LOW);
  Serial.println("GPIO 26 toggled");
}

void loop() {}
```

## Alternative Pin Configurations

### If GPIO 22 is Not Available

If your board variant doesn't have GPIO 22 accessible:

**Option 1:** Use GPIO 21 (but conflicts with I2C SDA)
```cpp
#define I2S_LRC  21
```

**Option 2:** Use GPIO 32 (but conflicts with I2C SCL)
```cpp
#define I2S_LRC  32
```

**Option 3:** Use RGB LED pins (if not needed)
```cpp
#define I2S_LRC  4   // Red LED pin
// or
#define I2S_LRC  16  // Green LED pin
// or
#define I2S_LRC  17  // Blue LED pin
```

### Stereo Configuration Pins

For stereo output (two MAX98357A modules):

```
LEFT Channel (SD → GND):
- Shared: GPIO 22 (LRC), GPIO 25 (DIN), GPIO 26 (BCLK)
- Power: 5V, GND

RIGHT Channel (SD → VIN):
- Shared: GPIO 22 (LRC), GPIO 25 (DIN), GPIO 26 (BCLK)
- Power: 5V, GND

Both amplifiers share all I2S signals.
Channel selection is done via SD pin connection.
```

## Common Issues

### Issue: No Audio Output

**Check:**
1. GPIO 25, 26, 22 are connected correctly
2. Not using GPIO 27 by mistake
3. MAX98357A has power (VIN = 5V, GND connected)
4. Speaker connected to SPEAKER+/-

### Issue: Display Backlight Flickering

**Cause:** Using GPIO 27 for I2S_LRC

**Solution:** Change to GPIO 22 in config.h

### Issue: Can't Find GPIO 22

**Check:**
1. Look for CN1 connector (top right of board)
2. May be labeled as "P1" or "EXT" on some variants
3. Consult your specific board's pinout diagram

## Board Variants

Different manufacturers may have slightly different pin breakouts:

**Standard CYD (Most Common):**
- All pins accessible via headers
- CN1 has GPIO 22
- P3 has GPIO 25, 26

**Budget Variants:**
- May not have all headers populated
- GPIO 22 may require soldering to ESP32 pin directly
- Check your specific board documentation

**Solution:** If headers are missing, you can:
1. Solder wires directly to ESP32 chip pins
2. Solder headers to existing pads
3. Use alternative pins (see above)

## Resources

- [Official ESP32 Pinout](https://randomnerdtutorials.com/esp32-pinout-reference-gpios/)
- [CYD GitHub Repository](https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display)
- [ESP32 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32_datasheet_en.pdf)

---

**Summary:** Use GPIO 22, 25, and 26 for I2S audio. Never use GPIO 27 (it's the backlight).
