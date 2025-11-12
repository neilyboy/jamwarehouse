# Hardware Assembly Guide

Complete step-by-step guide for assembling your Jam Wysteria Internet Radio Boombox.

## Table of Contents

1. [Tools and Materials](#tools-and-materials)
2. [Wiring the Audio Amplifier](#wiring-the-audio-amplifier)
3. [Connecting the Speaker](#connecting-the-speaker)
4. [Power Considerations](#power-considerations)
5. [Optional: Stereo Configuration](#optional-stereo-configuration)
6. [Enclosure Design](#enclosure-design)
7. [Testing and Troubleshooting](#testing-and-troubleshooting)

## Tools and Materials

### Required Tools
- Soldering iron (temperature controlled, 350-400°C recommended)
- Solder (60/40 or lead-free)
- Wire strippers
- Small screwdriver set
- Multimeter (for testing continuity)
- Helping hands or PCB holder
- Heat shrink tubing or electrical tape

### Required Materials
- 22-24 AWG solid or stranded wire (various colors recommended)
- Dupont connectors (optional, for non-permanent connections)
- Heat shrink tubing (various sizes)
- Small zip ties for cable management

### Safety Equipment
- Safety glasses
- Ventilated workspace or fume extractor
- ESD wrist strap (optional but recommended)

## Wiring the Audio Amplifier

### MAX98357A Pinout

```
MAX98357A I2S Audio Amplifier
┌─────────────────────┐
│  VIN  ●●  SPEAKER+  │
│  GND  ●●  SPEAKER-  │
│  SD   ●●  GAIN      │
│  DIN  ●●  (empty)   │
│  BCLK ●●  (empty)   │
│  LRC  ●●  (empty)   │
└─────────────────────┘
```

### ESP32-2432S028R Pinout (Relevant Pins)

```
ESP32-2432S028R (Expansion Pins)
┌───────────────────────────┐
│                           │
│   [USB-C Port]            │
│                           │
│  GPIO 22  ●  (I2S LRC)    │
│  GPIO 25  ●  (I2S DOUT)   │
│  GPIO 26  ●  (I2S BCLK)   │
│  GPIO 27  ●  (BACKLIGHT)  │
│  5V       ●               │
│  GND      ●               │
│                           │
└───────────────────────────┘
```

### Wiring Diagram (Mono Configuration)

```
ESP32-2432S028R          MAX98357A          Speaker
───────────────          ─────────          ───────

GPIO 22 ────────────────→ LRC
GPIO 25 ────────────────→ DIN
GPIO 26 ────────────────→ BCLK
5V      ────────────────→ VIN
GND     ────────────────→ GND
                          SD ───────────→ VIN (always on)
                          GAIN ─────────→ GND (9dB) or FLOAT (15dB)
                          
                          SPEAKER+ ─────→ Speaker (+) Red
                          SPEAKER- ─────→ Speaker (-) Black
                          
NOTE: GPIO 27 is used for display backlight - do NOT connect to MAX98357A!
```

### Step-by-Step Wiring

#### Step 1: Prepare the Wires

1. Cut six pieces of wire (approximately 15cm each):
   - 1x Red (5V)
   - 1x Black (GND)
   - 3x Different colors for I2S signals (Blue, Green, Yellow recommended)
   - 1x For SD pin (Orange recommended)

2. Strip 5mm of insulation from each end

3. Tin the ends with solder for easier connection

#### Step 2: Solder to ESP32 Breakout Pins

**IMPORTANT:** The ESP32-2432S028R has breakout pads on the bottom. You'll need to solder directly to these pads or use the available headers.

1. **GPIO 25 (I2S DOUT):**
   - Locate GPIO 25 pad/header
   - Solder blue wire to GPIO 25

2. **GPIO 26 (I2S BCLK):**
   - Locate GPIO 26 pad/header
   - Solder green wire to GPIO 26

3. **GPIO 22 (I2S LRC):**
   - Locate GPIO 22 on expansion header or CN1 connector
   - Solder yellow wire to GPIO 22
   - **NOTE:** GPIO 27 is used for display backlight - we use GPIO 22 instead for I2S_LRC

4. **5V:**
   - Solder red wire to 5V pad/header

5. **GND:**
   - Solder black wire to GND pad/header

**Tip:** Use different colored wires to avoid confusion. Label each wire with masking tape if needed.

#### Step 3: Connect to MAX98357A

1. **VIN (Power):**
   - Connect red wire from ESP32 5V to MAX98357A VIN
   - Ensure solid connection

2. **GND (Ground):**
   - Connect black wire from ESP32 GND to MAX98357A GND

3. **DIN (I2S Data):**
   - Connect blue wire from ESP32 GPIO 25 to MAX98357A DIN

4. **BCLK (I2S Bit Clock):**
   - Connect green wire from ESP32 GPIO 26 to MAX98357A BCLK

5. **LRC (I2S Word Select / Left-Right Clock):**
   - Connect yellow wire from ESP32 GPIO 22 to MAX98357A LRC

6. **SD (Shutdown):**
   - Bridge MAX98357A SD pin to VIN pin for always-on operation
   - Alternative: Connect to a GPIO for software shutdown control

7. **GAIN (Optional):**
   - Leave floating for 15dB gain (louder, more distortion)
   - Connect to GND for 9dB gain (quieter, cleaner)
   - Connect to VIN for 12dB gain (middle ground)

#### Step 4: Verify Connections

Before powering on, use a multimeter to check:

1. **Continuity:**
   - Verify each wire connection with continuity test
   - Check for no shorts between adjacent pins

2. **No Short Circuits:**
   - Verify no connection between 5V and GND
   - Check for no shorts on I2S signal lines

3. **Visual Inspection:**
   - Inspect all solder joints for cold joints or bridges
   - Ensure no loose wires

## Connecting the Speaker

### Speaker Specifications

**Recommended:**
- Impedance: 4-8Ω
- Power: 3W minimum, 5W recommended
- Type: Full-range driver or suitable for voice/music

**Compatible Speakers:**
- Small bookshelf speakers
- Laptop/tablet speakers (salvaged)
- 3W-5W project speakers from electronics suppliers
- Car door speakers (4Ω)

### Speaker Wiring

```
MAX98357A                    Speaker
─────────                    ───────

SPEAKER+ (Red Wire) ────────→ + Terminal (Red/Marked)
SPEAKER- (Black Wire) ──────→ - Terminal (Black/Unmarked)
```

### Connection Steps

1. **Identify Speaker Terminals:**
   - Most speakers have red (+) and black (-) terminals
   - If unmarked, test with a 1.5V AA battery (positive contact = cone moves out)

2. **Prepare Speaker Wires:**
   - Use 18-22 AWG speaker wire
   - Keep length reasonable (15-30cm for internal mounting)
   - Strip 5mm from each end

3. **Connect to MAX98357A:**
   - Solder or screw terminal connection to SPEAKER+ and SPEAKER-
   - Ensure polarity is correct for proper audio phase

4. **Secure Connection:**
   - Use heat shrink or electrical tape to insulate connections
   - Ensure wires won't short against other components

### Testing Speaker Polarity

If you have two speakers and want to ensure they're in phase:

1. Connect both speakers to separate amplifiers
2. Play same audio signal to both
3. If bass is weak, one speaker is out of phase
4. Reverse connections on one speaker to fix

## Power Considerations

### Power Requirements

**ESP32-2432S028R:**
- Typical: 80-160mA (WiFi active)
- Peak: 240mA (WiFi transmitting)
- Sleep: 10-20mA (deep sleep)

**MAX98357A + Speaker:**
- Idle: ~10mA
- Average playback (moderate volume): 200-500mA
- Peak (loud passages): 600mA+

**Total System:**
- Idle: ~100mA
- Normal use: 300-700mA
- Peak: 1000mA (1A)

**Recommended Power Supply:**
- **USB-C Power Adapter:** 5V 2A (minimum) or 5V 2.4A (recommended)
- **USB Power Bank:** 10,000mAh+ with 2A output for portable use
- **Wall Adapter:** Quality brand (Anker, Belkin, etc.)

### Portable Operation

For portable/battery operation:

1. **USB Power Bank:**
   - Capacity: 10,000mAh+ recommended
   - Output: 5V 2A or higher
   - Runtime estimate: 6-12 hours depending on volume

2. **Battery Installation:**
   ```
   Power Bank
   ───────────
        │
        │ USB-C Cable
        ↓
   ESP32-2432S028R
   ```

3. **Power Saving Tips:**
   - Reduce screen brightness
   - Lower volume level
   - Enable screen timeout
   - Use efficient streaming codecs

### Power Quality

**Important:** Poor quality power supplies can cause:
- Audio noise/hissing
- Display flickering
- WiFi disconnections
- System instability

**Solutions:**
- Use quality USB power adapter
- Add 100-470µF capacitor across power rails (optional)
- Keep power wires short and thick
- Separate digital and analog grounds (advanced)

## Optional: Stereo Configuration

For stereo sound, use two MAX98357A amplifiers:

### Stereo Wiring Diagram

```
ESP32-2432S028R     LEFT Amplifier       RIGHT Amplifier
───────────────     ──────────────       ───────────────

GPIO 25 ───┬───────→ DIN                    DIN ←─────────┐
           │                                               │
           └──────────────────────────────────────────────┘
           (shared data)

GPIO 26 ───┬───────→ BCLK                   BCLK ←────────┐
           │                                               │
           └──────────────────────────────────────────────┘
           (shared clock)

GPIO 22 ───┬───────→ LRC                    LRC ←─────────┐
           │                                               │
           └──────────────────────────────────────────────┘
           (shared word select)

5V ────────┬───────→ VIN                    VIN ←─────────┐
           │                                               │
           └──────────────────────────────────────────────┘
           (shared power)

GND ───────┬───────→ GND                    GND ←─────────┐
           │                                               │
           └──────────────────────────────────────────────┘
           (shared ground)

                     SD → GND               SD → VIN
                     (left channel)         (right channel)
                     
                     SPEAKER+ → Left+       SPEAKER+ → Right+
                     SPEAKER- → Left-       SPEAKER- → Right-
```

### Stereo Configuration Notes

1. **Channel Selection:**
   - LEFT channel: Connect SD to GND
   - RIGHT channel: Connect SD to VIN

2. **Shared Signals:**
   - All I2S signals (DIN, BCLK, LRC) are shared
   - Power (VIN, GND) is shared
   - Both amplifiers decode from same data stream

3. **Power Requirements:**
   - Double the current requirements
   - Use 5V 3A power supply minimum

## Enclosure Design

### Enclosure Options

1. **3D Printed:**
   - Custom fit for all components
   - Can design speaker grills
   - Recommended: PETG or ABS for durability

2. **Laser Cut Acrylic:**
   - Professional appearance
   - Good acoustic properties
   - Easy to modify

3. **Project Box:**
   - Hammond 1591XX series
   - Polycase boxes
   - Need to drill holes for speaker/ports

4. **DIY Wood:**
   - Classic speaker box design
   - Better acoustic properties
   - Requires woodworking skills

### Design Considerations

**Minimum Internal Dimensions:**
- Width: 120mm
- Height: 100mm
- Depth: 40mm

**Required Cutouts/Holes:**
- Display viewing area: 60mm x 50mm
- USB-C port access: 10mm x 5mm
- Speaker grill: 60mm diameter (or rectangular equivalent)
- Optional: Volume buttons, power switch
- Ventilation holes for heat dissipation

**Assembly Tips:**
- Mount ESP32 on standoffs to prevent shorts
- Secure amplifier with hot glue or standoffs
- Use foam tape for speaker mounting (reduces vibration)
- Cable management: use small zip ties
- Add rubber feet to bottom

### 3D Printable Enclosure

*Coming soon: STL files will be available in the `/enclosure` directory*

**Recommended Print Settings:**
- Layer height: 0.2mm
- Infill: 20%
- Material: PETG or ABS
- Supports: Yes (for overhangs)

## Testing and Troubleshooting

### Initial Power-On Test

1. **Visual Inspection:**
   - Check all connections before applying power
   - Verify no shorts with multimeter

2. **First Power-On:**
   - Connect USB-C cable (without speaker initially)
   - Look for display backlight
   - Check serial monitor for boot messages

3. **Speaker Connection:**
   - Power off
   - Connect speaker
   - Power on and test audio

### Troubleshooting Common Issues

#### No Display

**Symptoms:** Black screen, no backlight

**Solutions:**
- Check TFT_BL pin connection (GPIO 27)
- Verify 5V power to ESP32
- Check TFT_eSPI library configuration
- Verify SPI pin definitions in code

#### No Audio

**Symptoms:** No sound from speaker

**Solutions:**
- Check I2S pin connections (GPIO 25, 26, 27)
- Verify speaker polarity
- Test speaker with 1.5V battery (should make click sound)
- Check MAX98357A LED (should light when audio playing)
- Verify audio stream URL is working
- Check volume level in code

#### Distorted Audio

**Symptoms:** Crackling, popping, or distorted sound

**Solutions:**
- Use better quality power supply
- Add capacitor across power rails
- Reduce volume level
- Check speaker impedance (should be 4-8Ω)
- Adjust GAIN setting on MAX98357A
- Check for loose wire connections

#### WiFi Issues

**Symptoms:** Can't connect to WiFi, frequent disconnections

**Solutions:**
- Check WiFi credentials
- Verify 2.4GHz network (ESP32 doesn't support 5GHz)
- Move closer to router
- Check power supply quality
- Reduce power consumption (lower display brightness)

#### Touch Screen Not Responding

**Symptoms:** Can't interact with touch screen

**Solutions:**
- Verify touch calibration values
- Check XPT2046 library configuration
- Test with simple touch demo sketch
- Verify TOUCH_CS pin (GPIO 33)
- Check for screen protector thickness

### Performance Testing

#### Audio Quality Test

1. Play test tones (sine waves at 100Hz, 1kHz, 10kHz)
2. Listen for distortion or noise
3. Test at various volume levels
4. Verify stereo separation (if stereo)

#### Stability Test

1. Play continuous stream for 1+ hour
2. Monitor for WiFi disconnections
3. Check for audio dropouts
4. Monitor device temperature

#### Power Consumption Test

1. Use USB power meter
2. Measure idle current
3. Measure playing current at various volumes
4. Calculate battery life estimate

### Recommended Test Streams

Use these test streams to verify functionality:

**Good Quality Streams:**
- BBC Radio 1: `http://stream.live.vc.bbcmedia.co.uk/bbc_radio_one`
- NPR: Various stations available
- Local radio stations (check radio-browser.info)

**Test Stream Formats:**
- Direct MP3 stream
- M3U8 playlist
- HLS stream
- Shoutcast/Icecast

## Safety Notes

⚠️ **Important Safety Information:**

- Always disconnect power before making changes
- Use appropriate wire gauge for current requirements
- Ensure all connections are insulated
- Don't exceed speaker power ratings
- Provide adequate ventilation
- Don't expose to moisture
- Use proper soldering ventilation

## Next Steps

After assembly:

1. **Upload Firmware:**
   - Follow instructions in main README.md
   - Configure WiFi
   - Test all functions

2. **Add Stations:**
   - Use web interface for easy station management
   - Or add via touch screen keyboard
   - Import station list from provided examples

3. **Customize:**
   - Add your favorite radio stations
   - Organize stations into folders
   - Upload custom station logos to SD card

4. **Share:**
   - Post your build on maker forums
   - Contribute improvements back to project
   - Help others with their builds

## Support

Need help? Check:
- [GitHub Issues](https://github.com/yourusername/jam-wysteria/issues)
- [Discussions](https://github.com/yourusername/jam-wysteria/discussions)
- [Troubleshooting Guide](TROUBLESHOOTING.md)

Happy building! 🎵
