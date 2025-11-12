/**
 * Jam Wysteria - ESP32 Internet Radio Boombox
 * 
 * A modern touchscreen internet radio player for ESP32-2432S028R
 * (Cheap Yellow Display) with web-based station management.
 * 
 * Hardware: ESP32-2432S028R + MAX98357A I2S Amplifier
 * Display: 2.8" ILI9341 320x240 TFT with touch
 * Audio: I2S digital output to Class D amplifier
 * Storage: MicroSD card for logos and configuration
 * 
 * Features:
 * - WiFi setup wizard with on-screen keyboard
 * - Hierarchical folder/station organization
 * - Touch-driven UI with modern design
 * - Web interface for easy station management
 * - Support for various streaming formats
 * - Metadata display (artist, title, album art)
 * - Volume control and player features
 * 
 * @author Your Name
 * @version 1.0.0
 * @date 2024
 */

#include "config.h"
#include "display.h"
#include "touch.h"
#include "wifi_manager.h"
#include "audio_player.h"
#include "station_manager.h"
#include "web_server.h"
#include "ui_manager.h"
#include "sd_manager.h"

// Global state
AppState currentState = STATE_BOOT;
AppState previousState = STATE_BOOT;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n╔══════════════════════════════════════╗");
  Serial.println("║     JAM WYSTERIA INTERNET RADIO      ║");
  Serial.println("║         v1.0.0 - Booting...          ║");
  Serial.println("╚══════════════════════════════════════╝\n");
  
  // Initialize display first (shows splash screen)
  Serial.println("[INIT] Initializing display...");
  Display.init();
  Display.showSplashScreen();
  
  // Initialize touch
  Serial.println("[INIT] Initializing touch screen...");
  Touch.init();
  
  // Initialize SD card
  Serial.println("[INIT] Initializing SD card...");
  if (!SDManager.init()) {
    Serial.println("[ERROR] SD card initialization failed!");
    Display.showError("SD Card Error", "Please insert SD card and restart");
    while(1) delay(1000); // Halt
  }
  
  // Load configuration from SD
  Serial.println("[INIT] Loading configuration...");
  SDManager.loadConfig();
  
  // Initialize WiFi manager
  Serial.println("[INIT] Initializing WiFi...");
  WiFiManager.init();
  
  // Check if WiFi credentials are stored
  if (!WiFiManager.hasCredentials()) {
    Serial.println("[INFO] No WiFi credentials found. Starting setup wizard...");
    currentState = STATE_WIFI_SETUP;
  } else {
    Serial.println("[INFO] WiFi credentials found. Connecting...");
    Display.showMessage("Connecting to WiFi...");
    
    if (WiFiManager.connect()) {
      Serial.println("[INFO] WiFi connected!");
      Serial.print("[INFO] IP Address: ");
      Serial.println(WiFiManager.getIPAddress());
      
      currentState = STATE_HOME;
    } else {
      Serial.println("[WARN] WiFi connection failed. Starting setup wizard...");
      currentState = STATE_WIFI_SETUP;
    }
  }
  
  // Initialize station manager
  Serial.println("[INIT] Initializing station manager...");
  StationManager.init();
  StationManager.loadStations();
  
  // Initialize audio player
  Serial.println("[INIT] Initializing audio player...");
  AudioPlayer.init();
  
  // Initialize web server
  Serial.println("[INIT] Starting web server...");
  WebServer.init();
  WebServer.begin();
  
  // Initialize UI manager
  Serial.println("[INIT] Initializing UI manager...");
  UIManager.init();
  
  Serial.println("[INIT] ✓ Initialization complete!\n");
  
  // Show appropriate screen based on state
  updateScreen();
}

void loop() {
  // Update components
  Touch.update();
  AudioPlayer.update();
  WiFiManager.update();
  
  // Handle touch events
  if (Touch.isTouched()) {
    TouchPoint point = Touch.getPoint();
    handleTouch(point);
  }
  
  // Handle state-specific updates
  switch (currentState) {
    case STATE_WIFI_SETUP:
      // WiFi setup wizard is handled by touch events
      break;
      
    case STATE_HOME:
      // Update station list if needed
      UIManager.updateHomeScreen();
      break;
      
    case STATE_PLAYING:
      // Update player screen with metadata
      if (AudioPlayer.isPlaying()) {
        UIManager.updatePlayerScreen();
      }
      break;
      
    case STATE_SETTINGS:
      // Settings screen updates
      UIManager.updateSettingsScreen();
      break;
      
    case STATE_FOLDER_VIEW:
      // Folder contents view
      UIManager.updateFolderView();
      break;
      
    case STATE_KEYBOARD:
      // On-screen keyboard is handled by touch events
      break;
  }
  
  // Small delay to prevent overwhelming the CPU
  delay(10);
}

/**
 * Handle touch events based on current state
 */
void handleTouch(TouchPoint point) {
  #ifdef DEBUG_MODE
  Serial.printf("[TOUCH] X: %d, Y: %d (State: %d)\n", point.x, point.y, currentState);
  #endif
  
  switch (currentState) {
    case STATE_WIFI_SETUP:
      handleWiFiSetupTouch(point);
      break;
      
    case STATE_HOME:
      handleHomeTouch(point);
      break;
      
    case STATE_PLAYING:
      handlePlayerTouch(point);
      break;
      
    case STATE_SETTINGS:
      handleSettingsTouch(point);
      break;
      
    case STATE_FOLDER_VIEW:
      handleFolderViewTouch(point);
      break;
      
    case STATE_KEYBOARD:
      handleKeyboardTouch(point);
      break;
      
    case STATE_ADD_STATION:
      handleAddStationTouch(point);
      break;
      
    case STATE_ADD_FOLDER:
      handleAddFolderTouch(point);
      break;
  }
}

/**
 * Handle touch events on WiFi setup screen
 */
void handleWiFiSetupTouch(TouchPoint point) {
  int action = UIManager.checkWiFiSetupTouch(point);
  
  switch (action) {
    case WIFI_ACTION_SCAN:
      Display.showMessage("Scanning networks...");
      WiFiManager.scanNetworks();
      UIManager.showWiFiNetworkList();
      break;
      
    case WIFI_ACTION_SELECT_NETWORK:
      {
        int networkIndex = UIManager.getSelectedNetwork(point);
        if (networkIndex >= 0) {
          String ssid = WiFiManager.getScannedSSID(networkIndex);
          UIManager.showPasswordEntry(ssid);
        }
      }
      break;
      
    case WIFI_ACTION_CONNECT:
      {
        String ssid = UIManager.getCurrentSSID();
        String password = UIManager.getCurrentPassword();
        
        Display.showMessage("Connecting to " + ssid + "...");
        
        if (WiFiManager.connect(ssid, password)) {
          Display.showMessage("Connected!", 2000);
          WiFiManager.saveCredentials(ssid, password);
          
          // Transition to home screen
          currentState = STATE_HOME;
          updateScreen();
        } else {
          Display.showError("Connection Failed", "Please check password and try again");
          delay(3000);
          UIManager.showPasswordEntry(ssid);
        }
      }
      break;
      
    case WIFI_ACTION_BACK:
      UIManager.showWiFiNetworkList();
      break;
      
    case WIFI_ACTION_KEYBOARD:
      // Keyboard touch handling is done in keyboard mode
      break;
  }
}

/**
 * Handle touch events on home screen
 */
void handleHomeTouch(TouchPoint point) {
  int action = UIManager.checkHomeScreenTouch(point);
  
  switch (action) {
    case HOME_ACTION_ADD:
      // Show add menu (folder or station)
      UIManager.showAddMenu();
      currentState = STATE_ADD_STATION;
      break;
      
    case HOME_ACTION_SETTINGS:
      currentState = STATE_SETTINGS;
      UIManager.showSettingsScreen();
      break;
      
    case HOME_ACTION_FOLDER:
      {
        int folderIndex = UIManager.getSelectedFolder(point);
        if (folderIndex >= 0) {
          StationManager.enterFolder(folderIndex);
          currentState = STATE_FOLDER_VIEW;
          UIManager.showFolderView();
        }
      }
      break;
      
    case HOME_ACTION_STATION:
      {
        int stationIndex = UIManager.getSelectedStation(point);
        if (stationIndex >= 0) {
          Station* station = StationManager.getStation(stationIndex);
          if (station != nullptr) {
            playStation(station);
          }
        }
      }
      break;
  }
}

/**
 * Handle touch events on player screen
 */
void handlePlayerTouch(TouchPoint point) {
  int action = UIManager.checkPlayerTouch(point);
  
  switch (action) {
    case PLAYER_ACTION_PLAY_PAUSE:
      if (AudioPlayer.isPlaying()) {
        AudioPlayer.pause();
      } else {
        AudioPlayer.resume();
      }
      UIManager.updatePlayerScreen();
      break;
      
    case PLAYER_ACTION_STOP:
      AudioPlayer.stop();
      currentState = STATE_HOME;
      updateScreen();
      break;
      
    case PLAYER_ACTION_VOLUME_UP:
      AudioPlayer.volumeUp();
      UIManager.updatePlayerScreen();
      break;
      
    case PLAYER_ACTION_VOLUME_DOWN:
      AudioPlayer.volumeDown();
      UIManager.updatePlayerScreen();
      break;
      
    case PLAYER_ACTION_NEXT:
      // Next track (if supported by stream)
      AudioPlayer.next();
      break;
      
    case PLAYER_ACTION_PREVIOUS:
      // Previous track (if supported by stream)
      AudioPlayer.previous();
      break;
      
    case PLAYER_ACTION_BACK:
      AudioPlayer.stop();
      currentState = previousState;
      updateScreen();
      break;
  }
}

/**
 * Handle touch events on settings screen
 */
void handleSettingsTouch(TouchPoint point) {
  int action = UIManager.checkSettingsTouch(point);
  
  switch (action) {
    case SETTINGS_ACTION_WIFI:
      currentState = STATE_WIFI_SETUP;
      UIManager.showWiFiNetworkList();
      break;
      
    case SETTINGS_ACTION_DISPLAY:
      UIManager.showDisplaySettings();
      break;
      
    case SETTINGS_ACTION_AUDIO:
      UIManager.showAudioSettings();
      break;
      
    case SETTINGS_ACTION_ABOUT:
      UIManager.showAboutScreen();
      break;
      
    case SETTINGS_ACTION_BACK:
      currentState = STATE_HOME;
      updateScreen();
      break;
  }
}

/**
 * Handle touch events on folder view screen
 */
void handleFolderViewTouch(TouchPoint point) {
  int action = UIManager.checkFolderViewTouch(point);
  
  switch (action) {
    case FOLDER_ACTION_BACK:
      if (StationManager.canGoBack()) {
        StationManager.goBack();
        UIManager.showFolderView();
      } else {
        currentState = STATE_HOME;
        updateScreen();
      }
      break;
      
    case FOLDER_ACTION_FOLDER:
      {
        int folderIndex = UIManager.getSelectedFolder(point);
        if (folderIndex >= 0) {
          StationManager.enterFolder(folderIndex);
          UIManager.showFolderView();
        }
      }
      break;
      
    case FOLDER_ACTION_STATION:
      {
        int stationIndex = UIManager.getSelectedStation(point);
        if (stationIndex >= 0) {
          Station* station = StationManager.getStation(stationIndex);
          if (station != nullptr) {
            playStation(station);
          }
        }
      }
      break;
  }
}

/**
 * Handle touch events on keyboard
 */
void handleKeyboardTouch(TouchPoint point) {
  char key = UIManager.getKeyboardKey(point);
  
  if (key != 0) {
    if (key == '\n') {
      // Enter key pressed - submit input
      UIManager.submitKeyboardInput();
      currentState = previousState;
      updateScreen();
    } else if (key == '\b') {
      // Backspace
      UIManager.keyboardBackspace();
    } else {
      // Regular character
      UIManager.keyboardAddChar(key);
    }
    
    UIManager.updateKeyboard();
  }
}

/**
 * Handle touch events on add station/folder screen
 */
void handleAddStationTouch(TouchPoint point) {
  int action = UIManager.checkAddMenuTouch(point);
  
  switch (action) {
    case ADD_ACTION_FOLDER:
      currentState = STATE_ADD_FOLDER;
      UIManager.showAddFolderScreen();
      break;
      
    case ADD_ACTION_STATION:
      currentState = STATE_ADD_STATION;
      UIManager.showAddStationScreen();
      break;
      
    case ADD_ACTION_BACK:
      currentState = STATE_HOME;
      updateScreen();
      break;
  }
}

/**
 * Handle touch events on add folder screen
 */
void handleAddFolderTouch(TouchPoint point) {
  // Implementation for add folder touch handling
  // This would involve keyboard input for folder name and icon selection
}

/**
 * Play a station
 */
void playStation(Station* station) {
  if (station == nullptr) return;
  
  Serial.print("[PLAY] Starting station: ");
  Serial.println(station->name);
  
  Display.showMessage("Loading " + station->name + "...");
  
  if (AudioPlayer.playStation(station->url)) {
    previousState = currentState;
    currentState = STATE_PLAYING;
    UIManager.showPlayerScreen(station);
    
    // Save as last played station
    SDManager.saveLastStation(station->name);
  } else {
    Display.showError("Playback Error", "Could not play " + station->name);
    delay(3000);
    updateScreen();
  }
}

/**
 * Update screen based on current state
 */
void updateScreen() {
  switch (currentState) {
    case STATE_BOOT:
      Display.showSplashScreen();
      break;
      
    case STATE_WIFI_SETUP:
      UIManager.showWiFiSetupScreen();
      break;
      
    case STATE_HOME:
      UIManager.showHomeScreen();
      break;
      
    case STATE_PLAYING:
      // Player screen is shown when playStation() is called
      break;
      
    case STATE_SETTINGS:
      UIManager.showSettingsScreen();
      break;
      
    case STATE_FOLDER_VIEW:
      UIManager.showFolderView();
      break;
  }
}
