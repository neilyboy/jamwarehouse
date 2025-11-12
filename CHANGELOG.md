# Changelog

All notable changes to the Jam Wysteria project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Planned Features
- Album art display from stream metadata
- Equalizer controls with preset modes
- Sleep timer functionality
- Alarm/wake-up timer with station selection
- Favorites/bookmarks system
- Recently played stations list
- Station search functionality
- Bluetooth audio input support
- MQTT integration for home automation
- OTA (Over-The-Air) firmware updates
- Multi-language support
- Custom themes and color schemes
- Gesture controls (swipe, pinch)
- Voice control integration
- Podcast support with episode management
- Recording/time-shift functionality

## [1.0.0] - 2024-11-11

### Added
- **Initial Release** - Fully functional internet radio player
- WiFi setup wizard with network scanning and password entry
- On-screen QWERTY keyboard with shift and symbol modes
- Hierarchical folder and station organization system
- Touch-driven UI with modern card-based design
- I2S audio output to MAX98357A amplifier
- Support for multiple streaming formats (MP3, AAC, M3U8, PLS)
- Web interface for station management
  - Add/edit/delete stations and folders
  - Bulk import from JSON or CSV
  - Export station configuration
- MicroSD card support for:
  - Configuration storage
  - Station data persistence
  - Custom logos and icons
- Volume control with on-screen slider
- Settings menu with WiFi, display, and audio options
- Metadata display (artist, title, bitrate)
- Auto-reconnect for WiFi and streams
- Station logos from SD card
- Default emoji icons for stations and folders
- Serial debug output
- Splash screen on boot
- About screen with device information
- Admin panel for system management

### Hardware Support
- ESP32-2432S028R (Cheap Yellow Display)
- ILI9341 2.8" 320x240 TFT display
- XPT2046 touch screen controller
- MAX98357A I2S Class D amplifier
- MicroSD card slot
- 4-8Ω speakers (mono or stereo)

### Documentation
- Comprehensive README with features and installation
- Hardware assembly guide with wiring diagrams
- Quick start guide (30-minute setup)
- Contributing guidelines
- Detailed code comments
- Example station configurations (JSON and CSV)
- TFT_eSPI configuration template
- PlatformIO configuration

### Code Structure
- Modular architecture with separate managers:
  - Display Manager (TFT operations)
  - Touch Manager (input handling)
  - WiFi Manager (network operations)
  - Audio Player (streaming and playback)
  - Station Manager (data management)
  - SD Manager (storage operations)
  - UI Manager (screen rendering)
  - Web Server (HTTP interface)
  - Keyboard (on-screen input)
- State machine for screen navigation
- Non-blocking async operations
- Efficient memory management
- Error handling and recovery

### Known Issues
- Touch calibration may need adjustment for different display batches
- Some streams may have connection issues (depends on source reliability)
- Long station names may be truncated on screen
- Album art not yet implemented
- No playlist navigation for playlist-based streams

### Performance
- Typical memory usage: ~80KB RAM (out of ~320KB available)
- Boot time: ~3-5 seconds
- Stream connection: 2-10 seconds (network dependent)
- Touch response: <100ms
- Web interface response: <500ms

### Security Notes
- WiFi passwords stored with basic XOR encryption on SD card
- No authentication required for web interface (local network only)
- Consider network security for public deployments

## Version History

### Version Numbering
- **Major.Minor.Patch** (e.g., 1.0.0)
- **Major**: Breaking changes, major feature additions
- **Minor**: New features, backward compatible
- **Patch**: Bug fixes, minor improvements

---

## Future Roadmap

### v1.1.0 (Planned)
- Album art display from stream metadata
- Favorites/bookmarks system
- Recently played list
- Equalizer with presets
- OTA updates

### v1.2.0 (Planned)
- Sleep timer
- Alarm/wake-up timer
- Podcast support
- Station search

### v2.0.0 (Future)
- Bluetooth audio input
- Voice control
- MQTT/home automation
- Multi-language support
- Custom themes

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for how to contribute to this project.

## Links

- [GitHub Repository](https://github.com/yourusername/jam-wysteria)
- [Issue Tracker](https://github.com/yourusername/jam-wysteria/issues)
- [Discussions](https://github.com/yourusername/jam-wysteria/discussions)
