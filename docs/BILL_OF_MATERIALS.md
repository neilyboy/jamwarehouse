# Bill of Materials (BOM)

Complete parts list for building the Jam Wysteria Internet Radio Boombox.

## Required Components

### Electronics

| Item | Description | Specifications | Qty | Est. Cost | Purchase Links |
|------|-------------|----------------|-----|-----------|----------------|
| **ESP32-2432S028R** | Cheap Yellow Display development board | ESP32-WROOM-32, 2.8" TFT, Touch, SD slot | 1 | $15-20 | [Amazon](https://www.amazon.com/ELEGOO-Display-Acrylic-Protector-Projects/dp/B0FJQJZYXG), [AliExpress](https://aliexpress.com/item/1005004788147691.html) |
| **MAX98357A** | I2S Class D Amplifier Module | 3W output, I2S input, mono/stereo | 1 | $3-5 | [Amazon Option 1](https://www.amazon.com/MTDELE-MAX98357-Amplifier-Filterless-Compatible/dp/B0F3WYQWVG), [Amazon Option 2](https://www.amazon.com/MAX98357-MAX98357A-Amplifier-Interface-Raspberry/dp/B0DPJRLMDJ/), [Adafruit](https://www.adafruit.com/product/3006) |
| **Speaker** | 4-8Ω Miniature Speaker | 3W minimum, 5W recommended | 1-2 | $5-15 | [Amazon](https://www.amazon.com/s?k=4+ohm+3w+speaker), [Parts Express](https://www.parts-express.com/) |
| **MicroSD Card** | Flash Storage | Class 10, 4-32GB, FAT32 | 1 | $3-8 | [Amazon](https://www.amazon.com/s?k=microsd+card+class+10) |
| **USB-C Cable** | Data Cable | USB 2.0, data capable | 1 | $3-6 | [Amazon](https://www.amazon.com/s?k=usb+c+cable) |
| **Power Supply** | USB Wall Adapter | 5V 2A minimum, 2.4A recommended | 1 | $5-12 | [Amazon](https://www.amazon.com/s?k=5v+2a+usb+power+adapter) |

**Required Components Subtotal: $34-66**

---

## Wiring and Assembly Materials

| Item | Description | Specifications | Qty | Est. Cost | Purchase Links |
|------|-------------|----------------|-----|-----------|----------------|
| **Hookup Wire** | Solid or stranded wire | 22-24 AWG, multiple colors | 1m | $2-5 | [Amazon](https://www.amazon.com/s?k=22+awg+hookup+wire+kit) |
| **Solder** | 60/40 or Lead-free | 0.8-1mm diameter | 1 roll | $5-10 | [Amazon](https://www.amazon.com/s?k=electronics+solder) |
| **Heat Shrink Tubing** | Assorted sizes | 2-10mm diameters | 1 kit | $3-8 | [Amazon](https://www.amazon.com/s?k=heat+shrink+tubing+kit) |

**Wiring Materials Subtotal: $10-23**

---

## Optional Components

### For Better Performance

| Item | Description | Purpose | Qty | Est. Cost |
|------|-------------|---------|-----|-----------|
| **Electrolytic Capacitor** | 470µF 16V | Power supply filtering | 1-2 | $1-2 |
| **Ceramic Capacitor** | 100nF (0.1µF) | Decoupling | 2-4 | $1-2 |
| **Better Speaker** | 8Ω 5-10W | Improved audio quality | 1-2 | $10-20 |
| **Heatsink** | Small aluminum | Cool ESP32 if needed | 1 | $2-4 |

### For Portable Operation

| Item | Description | Specifications | Qty | Est. Cost |
|------|-------------|----------------|-----|-----------|
| **USB Power Bank** | Portable battery | 10,000+ mAh, 2A output | 1 | $15-30 |
| **USB-C to USB-A Cable** | For power bank connection | If power bank has USB-A only | 1 | $3-5 |

### For Stereo Configuration

| Item | Description | Purpose | Qty | Est. Cost |
|------|-------------|---------|-----|-----------|
| **MAX98357A** | Additional amplifier | Right channel | 1 | $3-5 |
| **Speaker** | Second speaker | Right channel | 1 | $5-15 |
| **Additional Wire** | For second amp/speaker | 22-24 AWG | 2m | $2-3 |

### Enclosure Options

| Option | Description | Materials | Est. Cost |
|--------|-------------|-----------|-----------|
| **3D Printed** | Custom designed case | PLA/PETG filament, ~100g | $5-10 (material) |
| **Project Box** | Plastic enclosure + drilling | Hammond 1591XX or similar | $8-15 |
| **Laser Cut Acrylic** | Clear or colored panels | 3mm acrylic sheets | $15-25 (service) |
| **DIY Wood** | Wooden speaker box | Small wood pieces, screws | $10-20 |

---

## Tools Required

### Essential Tools

| Tool | Purpose | Est. Cost (if buying) |
|------|---------|------------------------|
| **Soldering Iron** | Solder connections | $15-80 (recommend $30-50) |
| **Solder** | Join wires | $5-10 |
| **Wire Strippers** | Strip insulation | $8-15 |
| **Flush Cutters** | Cut wires | $5-12 |
| **Multimeter** | Test continuity, voltage | $10-25 |
| **Helping Hands** | Hold components while soldering | $8-15 |

**Tools Subtotal: $51-157** (one-time investment, reusable)

### Useful But Not Essential

| Tool | Purpose | Est. Cost |
|------|---------|-----------|
| **Heat Gun** | Shrink heat shrink tubing | $15-30 |
| **Desoldering Pump** | Fix mistakes | $5-10 |
| **Third Hand with Magnifier** | Better than basic helping hands | $12-25 |
| **Digital Calipers** | Measure enclosure dimensions | $10-20 |
| **Small Screwdriver Set** | General assembly | $8-15 |

---

## Where to Buy

### Online Retailers

**General Electronics:**
- **Amazon** - Fast shipping, good for complete kits
- **AliExpress** - Cheapest prices, longer shipping
- **Banggood** - Similar to AliExpress
- **eBay** - Mix of US and international sellers

**Specialized Electronics:**
- **Adafruit** - Quality components, great documentation
- **SparkFun** - Similar to Adafruit
- **DigiKey** - Professional components, fast shipping
- **Mouser** - Similar to DigiKey
- **Parts Express** - Excellent for speakers

**MicroSD Cards:**
- **Amazon** - Many options
- **Best Buy** - In-store pickup
- **Newegg** - Tech focused

**Local Options:**
- **Micro Center** - Electronics superstoreif near you
- **Fry's Electronics** - If still in your area
- **RadioShack** - Few stores remain
- **Local electronics hobby shops**

---

## Cost Breakdown by Configuration

### Minimal Build
**Purpose:** Get it working with minimum cost

| Category | Cost |
|----------|------|
| Required Components | $34-45 |
| Wiring Materials | $10-15 |
| Tools (if needed) | $51-100 |
| **Total** | **$95-160** |

**Note:** After first build, tools can be reused, so additional units cost only $44-60

---

### Recommended Build
**Purpose:** Good quality, reliable build

| Category | Cost |
|----------|------|
| Required Components | $40-55 |
| Wiring Materials | $12-18 |
| Optional Performance Parts | $5-10 |
| Better Speaker | $10-15 |
| Quality Power Supply | $8-12 |
| Basic Enclosure | $8-15 |
| Tools (if needed) | $60-120 |
| **Total** | **$143-245** |

---

### Premium Build  
**Purpose:** Best quality, portable, great sound

| Category | Cost |
|----------|------|
| Required Components | $45-66 |
| Wiring Materials | $15-23 |
| Performance Parts | $8-15 |
| Premium Speaker | $15-25 |
| Quality Power + Power Bank | $25-40 |
| Custom Enclosure | $15-30 |
| Tools (if needed) | $80-150 |
| **Total** | **$203-349** |

---

### Stereo Build
**Purpose:** Stereo sound output

| Category | Cost |
|----------|------|
| Recommended Build Base | $83-125 (no tools) |
| Second Amplifier | $3-5 |
| Second Speaker | $5-15 |
| Additional Wire | $2-3 |
| Larger Enclosure | $5-10 extra |
| **Total** | **$98-158** (without tools) |

---

## Shopping List Templates

### Copy-Paste Shopping List (Minimal)

```
[ ] 1x ESP32-2432S028R (Cheap Yellow Display)
[ ] 1x MAX98357A I2S Amplifier  
[ ] 1x 4Ω 3W Speaker
[ ] 1x MicroSD Card (Class 10, 4-8GB)
[ ] 1x USB-C Cable (data capable)
[ ] 1x 5V 2A USB Power Adapter
[ ] 1x Hookup Wire Kit (22 AWG, various colors)
[ ] Solder (if don't have)
[ ] Heat Shrink Tubing Kit
```

### Copy-Paste Shopping List (Recommended)

```
[ ] 1x ESP32-2432S028R (Cheap Yellow Display)
[ ] 1x MAX98357A I2S Amplifier
[ ] 1x 4-8Ω 5W Speaker (quality brand)
[ ] 1x MicroSD Card (Class 10, 8-16GB)
[ ] 1x USB-C Cable (braided, quality)
[ ] 1x 5V 2.4A USB Power Adapter (quality brand)
[ ] 1x Hookup Wire Kit (22 AWG, silicone insulation)
[ ] 1x Solder (60/40 or lead-free)
[ ] 1x Heat Shrink Tubing Kit
[ ] 2x 470µF 16V Electrolytic Capacitors
[ ] 4x 100nF Ceramic Capacitors
[ ] 1x Project Box or Enclosure
[ ] Small zip ties for cable management
[ ] USB Power Bank (10,000mAh, 2A+) for portable use
```

---

## Money-Saving Tips

### Save on Components

1. **Buy from AliExpress/Banggood:**
   - 30-50% cheaper than Amazon
   - Trade-off: 2-6 week shipping

2. **Bundle Purchases:**
   - Buy multiple ESP32 boards (build several units)
   - Share with friends/maker group

3. **Use Existing Parts:**
   - Salvage speaker from old device
   - Use existing USB cables and power adapters
   - Reuse SD cards you already have

4. **Wait for Sales:**
   - Amazon Prime Day
   - Black Friday / Cyber Monday
   - AliExpress sale days

### Save on Tools

1. **Borrow from Friends:**
   - Many makers have extra tools
   - Makerspaces often have tools to use

2. **Buy Quality Once:**
   - Good soldering iron lasts years
   - Cheap tools frustrate and may cost more long-term

3. **Harbor Freight:**
   - Budget tools that work for hobby use
   - Multimeter, wire strippers, cutters

4. **Watch for Deals:**
   - Tool sales at hardware stores
   - Used tools on eBay, Craigslist

---

## Component Alternatives

### If ESP32-2432S028R Unavailable

**Alternative Option 1:**
- ESP32 Dev Board + 2.8" ILI9341 TFT + XPT2046 Touch
- Cost: Similar, more complex wiring
- Advantage: More flexible pin assignments

**Alternative Option 2:**
- ESP32-S3 with built-in display (various models)
- Cost: $20-35
- Advantage: More powerful, USB-C, more RAM

### If MAX98357A Unavailable

**Alternative Amplifiers:**
- **PAM8403** - 3W Class D, cheaper but digital control
- **TDA7297** - More power but requires more components
- **MAX98357** (older version) - Nearly identical
- **LM386** - Classic amp, requires more external parts

### If Specific Speaker Unavailable

**Speaker Options:**
- Salvage from old electronics (laptop, tablet, portable speakers)
- Car door speakers (usually 4Ω)
- DIY speaker driver from electronics supplier
- Cheap PC/laptop speakers (can remove from enclosure)

**Important:** Stick to 4-8Ω impedance, 3W+ power rating

---

## Quality Recommendations

### Worth Spending Extra On:

1. **Power Supply:** Quality 2.4A+ adapter
   - Prevents brownouts, reboots, audio issues
   - Anker, Belkin, or Samsung brands

2. **Speaker:** Better speaker = better sound
   - Spend $10-20 instead of $5
   - Noticeable improvement

3. **Soldering Iron:** Temperature controlled
   - Makes soldering much easier
   - Prevents damage to components

4. **MicroSD Card:** Name brand Class 10
   - SanDisk, Samsung, Kingston
   - Fake cards common on cheap marketplaces

### OK to Go Cheap:

1. **Hookup Wire:** Generic is fine
2. **Heat Shrink:** Any brand works
3. **Enclosure:** DIY or basic project box
4. **USB Cable:** Any data-capable cable works

---

## Shipping and Availability Notes

### Shipping Times

- **Amazon Prime:** 1-2 days (US)
- **Amazon Standard:** 3-7 days
- **AliExpress:** 2-6 weeks (from China)
- **Banggood:** 2-6 weeks (from China)
- **DigiKey/Mouser:** 1-3 days

### Stock Availability

**High Availability (easy to find):**
- MAX98357A amplifiers
- Generic speakers
- MicroSD cards
- USB cables and power adapters
- Wire and solder

**Variable Availability:**
- **ESP32-2432S028R** - Popular, sometimes out of stock
  - Check multiple sellers
  - Similar models available with different seller codes

**Pro Tip:** Order ESP32 board first (longest lead time), gather other parts while waiting

---

## Bulk Buying (Multiple Units)

### Cost Per Unit When Building 5 Units:

| Component | Single Unit | 5 Units Bulk | Savings |
|-----------|-------------|--------------|---------|
| ESP32-2432S028R | $18 | $15 | $15 total |
| MAX98357A | $4 | $3 | $5 total |
| Speaker | $8 | $6 | $10 total |
| Misc Parts | $15 | $12 | $15 total |
| **Per Unit** | **$45** | **$36** | **$45 total** |

**Build as a group project and save!**

---

## Final Cost Summary

### Just the Radio (No Tools, Have Wire/Solder)
- **Minimum:** $34-45
- **Typical:** $40-55  
- **Premium:** $50-75

### Complete First Build (Including Tools)
- **Minimum:** $95-160
- **Typical:** $143-245
- **Premium:** $203-349

### Additional Units (Have Tools, Wire, Solder)
- **Each Additional:** $34-55

**Compared to Commercial Internet Radios:**
- Basic Models: $50-150
- Good Models: $150-300
- **Your DIY:** $34-75

**Savings: 30-80%** plus you get:
- Customization
- Learning experience
- Repair/upgrade ability
- Open source freedom

---

For the latest pricing and links, check the main [README.md](../README.md) or visit the [GitHub repository](https://github.com/yourusername/jam-wysteria).
