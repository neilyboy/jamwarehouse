/**
 * Jam Wysteria Configuration File
 * 
 * Hardware pin definitions, display settings, audio settings,
 * and application constants.
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================================================
// DEBUG SETTINGS
// ============================================================================
// Uncomment to enable debug output to serial monitor
#define DEBUG_MODE

// ============================================================================
// HARDWARE PIN DEFINITIONS - ESP32-2432S028R (Cheap Yellow Display)
// ============================================================================

// Display pins (ILI9341 via SPI)
#define TFT_CS          15
#define TFT_DC          2
#define TFT_RST         -1  // Not connected
#define TFT_MOSI        13
#define TFT_MISO        12
#define TFT_SCLK        14
#define TFT_LED         27  // Backlight control (can use PWM for brightness)

// Touch screen pins (XPT2046)
#define TOUCH_CS        33
#define TOUCH_IRQ       36  // Touch interrupt pin

// I2S Audio pins (MAX98357A)
#define I2S_DOUT        25  // DIN on MAX98357A
#define I2S_BCLK        26  // BCLK on MAX98357A
#define I2S_LRC         22  // LRC on MAX98357A (GPIO 22 available on expansion header)

// SD Card pins
#define SD_CS           5
#define SD_MOSI         23
#define SD_MISO         19
#define SD_SCLK         18

// RGB LED (optional - on board LED)
#define RGB_LED_R       4
#define RGB_LED_G       16
#define RGB_LED_B       17

// ============================================================================
// DISPLAY SETTINGS
// ============================================================================
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   240
#define SCREEN_ROTATION 1   // 0=0°, 1=90°, 2=180°, 3=270°

// Touch calibration values (may need adjustment for your specific display)
#define TOUCH_MIN_X     200
#define TOUCH_MAX_X     3800
#define TOUCH_MIN_Y     200
#define TOUCH_MAX_Y     3800

// Backlight PWM settings
#define BACKLIGHT_CHANNEL   0
#define BACKLIGHT_FREQ      5000
#define BACKLIGHT_RESOLUTION 8
#define BACKLIGHT_DEFAULT   200  // 0-255

// UI Colors (RGB565 format)
#define COLOR_BACKGROUND    0x0000  // Black
#define COLOR_PRIMARY       0x07FF  // Cyan
#define COLOR_SECONDARY     0xFD20  // Orange
#define COLOR_TEXT          0xFFFF  // White
#define COLOR_TEXT_DIM      0x8410  // Gray
#define COLOR_ACCENT        0xF81F  // Magenta
#define COLOR_SUCCESS       0x07E0  // Green
#define COLOR_ERROR         0xF800  // Red
#define COLOR_WARNING       0xFFE0  // Yellow
#define COLOR_CARD_BG       0x2104  // Dark gray
#define COLOR_BUTTON        0x4208  // Medium gray
#define COLOR_BUTTON_PRESS  0x6B4D  // Lighter gray

// ============================================================================
// AUDIO SETTINGS
// ============================================================================
#define AUDIO_BUFFER_SIZE   8192    // Audio buffer size in bytes
#define AUDIO_SAMPLE_RATE   44100   // Sample rate in Hz
#define AUDIO_BITS_PER_SAMPLE 16    // Bits per sample
#define AUDIO_CHANNELS      1       // 1=Mono, 2=Stereo

// Volume settings
#define VOLUME_MIN          0
#define VOLUME_MAX          21      // 0-21 for ESP32-audioI2S library
#define VOLUME_DEFAULT      12      // Default volume level
#define VOLUME_STEP         1       // Volume adjustment step

// Stream connection settings
#define STREAM_CONNECT_TIMEOUT  10000   // Connection timeout in ms
#define STREAM_RECONNECT_DELAY  5000    // Delay before reconnect attempt in ms
#define STREAM_MAX_RECONNECTS   3       // Max reconnection attempts

// ============================================================================
// WIFI SETTINGS
// ============================================================================
#define WIFI_CONNECT_TIMEOUT    20000   // WiFi connection timeout in ms
#define WIFI_SCAN_MAX_NETWORKS  20      // Maximum networks to display
#define WIFI_RECONNECT_DELAY    5000    // Delay before reconnect in ms

// ============================================================================
// WEB SERVER SETTINGS
// ============================================================================
#define WEB_SERVER_PORT     80          // HTTP port
#define WEB_SOCKET_PORT     81          // WebSocket port for real-time updates
#define API_ENDPOINT        "/api"      // API base path

// ============================================================================
// SD CARD SETTINGS
// ============================================================================
#define SD_MAX_PATH_LENGTH  256
#define SD_CONFIG_FILE      "/config/config.json"
#define SD_STATIONS_FILE    "/config/stations.json"
#define SD_LOGOS_DIR        "/logos"
#define SD_ICONS_DIR        "/icons"

// Default icons
#define DEFAULT_FOLDER_ICON     "📁"
#define DEFAULT_STATION_ICON    "📻"
#define DEFAULT_MUSIC_ICON      "🎵"
#define DEFAULT_NEWS_ICON       "📰"
#define DEFAULT_SPORTS_ICON     "⚽"
#define DEFAULT_TALK_ICON       "🎙️"

// ============================================================================
// UI SETTINGS
// ============================================================================
// Touch debounce
#define TOUCH_DEBOUNCE_MS   200     // Minimum time between touch events

// Scroll settings
#define SCROLL_ITEM_HEIGHT  50      // Height of each list item in pixels
#define SCROLL_MAX_ITEMS    10      // Max items visible at once
#define SCROLL_VELOCITY_MIN 5       // Minimum scroll velocity

// Keyboard settings
#define KEY_WIDTH           30      // Key width in pixels
#define KEY_HEIGHT          35      // Key height in pixels
#define KEY_SPACING         2       // Spacing between keys

// Animation settings
#define ANIMATION_DURATION  300     // Animation duration in ms
#define FADE_STEPS          10      // Steps for fade animations

// Splash screen duration
#define SPLASH_DURATION     3000    // Splash screen display time in ms

// ============================================================================
// APPLICATION STATES
// ============================================================================
enum AppState {
  STATE_BOOT,
  STATE_WIFI_SETUP,
  STATE_HOME,
  STATE_FOLDER_VIEW,
  STATE_PLAYING,
  STATE_SETTINGS,
  STATE_KEYBOARD,
  STATE_ADD_STATION,
  STATE_ADD_FOLDER,
  STATE_EDIT_STATION,
  STATE_EDIT_FOLDER
};

// ============================================================================
// UI ACTIONS
// ============================================================================
// WiFi setup actions
enum WiFiAction {
  WIFI_ACTION_NONE,
  WIFI_ACTION_SCAN,
  WIFI_ACTION_SELECT_NETWORK,
  WIFI_ACTION_CONNECT,
  WIFI_ACTION_BACK,
  WIFI_ACTION_KEYBOARD
};

// Home screen actions
enum HomeAction {
  HOME_ACTION_NONE,
  HOME_ACTION_ADD,
  HOME_ACTION_SETTINGS,
  HOME_ACTION_FOLDER,
  HOME_ACTION_STATION
};

// Player actions
enum PlayerAction {
  PLAYER_ACTION_NONE,
  PLAYER_ACTION_PLAY_PAUSE,
  PLAYER_ACTION_STOP,
  PLAYER_ACTION_NEXT,
  PLAYER_ACTION_PREVIOUS,
  PLAYER_ACTION_VOLUME_UP,
  PLAYER_ACTION_VOLUME_DOWN,
  PLAYER_ACTION_BACK
};

// Settings actions
enum SettingsAction {
  SETTINGS_ACTION_NONE,
  SETTINGS_ACTION_WIFI,
  SETTINGS_ACTION_DISPLAY,
  SETTINGS_ACTION_AUDIO,
  SETTINGS_ACTION_ABOUT,
  SETTINGS_ACTION_BACK
};

// Folder view actions
enum FolderAction {
  FOLDER_ACTION_NONE,
  FOLDER_ACTION_BACK,
  FOLDER_ACTION_FOLDER,
  FOLDER_ACTION_STATION
};

// Add menu actions
enum AddAction {
  ADD_ACTION_NONE,
  ADD_ACTION_FOLDER,
  ADD_ACTION_STATION,
  ADD_ACTION_BACK
};

// ============================================================================
// DATA STRUCTURES
// ============================================================================

// Touch point structure
struct TouchPoint {
  int x;
  int y;
  bool pressed;
};

// Station structure
struct Station {
  String name;
  String url;
  String iconPath;
  String parentFolder;
  int id;
};

// Folder structure
struct Folder {
  String name;
  String iconPath;
  String parentFolder;
  int id;
  int stationCount;
  int folderCount;
};

// Stream metadata
struct StreamMetadata {
  String title;
  String artist;
  String album;
  String genre;
  int bitrate;
  bool hasAlbumArt;
  String albumArtURL;
};

// Configuration structure
struct AppConfig {
  String wifiSSID;
  String wifiPassword;
  int volume;
  int brightness;
  String lastStation;
  bool autoConnect;
  int screenTimeout;
};

// ============================================================================
// VERSION INFO
// ============================================================================
#define APP_NAME        "Jam Wysteria"
#define APP_VERSION     "1.0.0"
#define APP_AUTHOR      "Your Name"
#define APP_URL         "https://github.com/yourusername/jam-wysteria"

#endif // CONFIG_H
