/**
 * UI Manager Implementation
 * 
 * Note: This is a comprehensive stub implementation showing the structure.
 * Full implementation of all drawing functions would be quite extensive.
 */

#include "ui_manager.h"
#include "station_manager.h"
#include "wifi_manager.h"
#include "audio_player.h"

// Global instance
UIManagerClass UIManager;

UIManagerClass::UIManagerClass() :
  currentSSID(""),
  currentPassword(""),
  scrollPosition(0),
  selectedIndex(-1) {
}

void UIManagerClass::init() {
  keyboard.init();
  Serial.println("[UI] Initialized");
}

// ============================================================================
// Screen Rendering
// ============================================================================

void UIManagerClass::showWiFiSetupScreen() {
  Display.clear();
  drawHeader("WiFi Setup");
  
  Display.drawCenteredText("Welcome to Jam Wysteria!", 60, COLOR_TEXT, 2);
  Display.drawCenteredText("Let's connect to WiFi", 90, COLOR_TEXT_DIM, 1);
  
  Display.drawButton(60, 140, 200, 50, "Scan Networks", COLOR_PRIMARY, COLOR_TEXT);
  
  buttons.clear();
  Button scanBtn = {60, 140, 200, 50, "Scan", WIFI_ACTION_SCAN, true};
  buttons.push_back(scanBtn);
}

void UIManagerClass::showWiFiNetworkList() {
  Display.clear();
  drawHeader("Select Network");
  drawBackButton();
  
  int networkCount = WiFiManager.getNetworkCount();
  int y = 50;
  
  buttons.clear();
  
  for (int i = 0; i < networkCount && i < 5; i++) {
    String ssid = WiFiManager.getScannedSSID(i);
    int rssi = WiFiManager.getScannedRSSI(i);
    bool secure = WiFiManager.isScannedSecure(i);
    
    drawWiFiNetwork(10, y, 300, ssid, rssi, secure);
    
    Button btn = {10, y, 300, 40, ssid, i, true};
    buttons.push_back(btn);
    
    y += 45;
  }
}

void UIManagerClass::showPasswordEntry(const String& ssid) {
  currentSSID = ssid;
  currentPassword = "";
  
  Display.clear();
  drawHeader("Enter Password");
  
  Display.drawText("Network: " + ssid, 10, 40, COLOR_TEXT, 1);
  
  // Show keyboard
  keyboard.clear();
  keyboard.show();
  
  // Add connect button
  Display.drawButton(10, 85, 100, 30, "Connect", COLOR_SUCCESS, COLOR_TEXT);
  drawBackButton();
}

void UIManagerClass::showHomeScreen() {
  Display.clear();
  drawHeader("Jam Wysteria");
  drawAddButton();
  drawSettingsButton();
  
  // Get folders and stations
  auto folders = StationManager.getCurrentFolders();
  auto stations = StationManager.getCurrentStations();
  
  buttons.clear();
  int y = 50;
  
  // Draw folders
  for (size_t i = 0; i < folders.size() && y < 200; i++) {
    drawFolderCard(10, y, 300, 40, folders[i]);
    
    Button btn = {10, y, 300, 40, folders[i]->name, (int)i, true};
    buttons.push_back(btn);
    
    y += 45;
  }
  
  // Draw stations
  for (size_t i = 0; i < stations.size() && y < 200; i++) {
    drawStationCard(10, y, 300, 40, stations[i]);
    
    Button btn = {10, y, 300, 40, stations[i]->name, (int)(i + 1000), true};
    buttons.push_back(btn);
    
    y += 45;
  }
  
  if (folders.size() == 0 && stations.size() == 0) {
    Display.drawCenteredText("No stations yet", 100, COLOR_TEXT_DIM, 2);
    Display.drawCenteredText("Press + to add", 130, COLOR_TEXT_DIM, 1);
  }
}

void UIManagerClass::showFolderView() {
  Display.clear();
  drawHeader(StationManager.getCurrentFolderName());
  drawBackButton();
  drawAddButton();
  
  // Similar to home screen but shows current folder contents
  showHomeScreen(); // Reuse home screen logic
}

void UIManagerClass::showPlayerScreen(Station* station) {
  if (station == nullptr) return;
  
  Display.clear();
  
  // Draw station name at top
  Display.drawCenteredText(station->name, 20, COLOR_PRIMARY, 2);
  
  // Draw album art / station logo (if available)
  int logoSize = 80;
  int logoX = (SCREEN_WIDTH - logoSize) / 2;
  int logoY = 50;
  
  if (station->iconPath.length() > 0) {
    Display.drawImageFromSD(station->iconPath, logoX, logoY, logoSize, logoSize);
  } else {
    // Draw default icon
    Display.fillRoundRect(logoX, logoY, logoSize, logoSize, 10, COLOR_CARD_BG);
    Display.drawIcon(logoX + 25, logoY + 25, "📻", COLOR_PRIMARY, 3);
  }
  
  // Draw metadata
  if (AudioPlayer.hasMetadata()) {
    drawMetadata(140, AudioPlayer.getTitle(), AudioPlayer.getArtist());
  } else {
    Display.drawCenteredText("Connecting...", 150, COLOR_TEXT_DIM, 1);
  }
  
  // Draw player controls
  int btnY = 180;
  int btnSize = 50;
  int btnSpacing = 20;
  int startX = 60;
  
  // Play/Pause button
  String playIcon = AudioPlayer.isPlaying() ? "⏸" : "▶";
  Display.drawButton(startX, btnY, btnSize, btnSize, playIcon, COLOR_PRIMARY, COLOR_TEXT);
  
  // Stop button
  Display.drawButton(startX + btnSize + btnSpacing, btnY, btnSize, btnSize, "⏹", COLOR_ERROR, COLOR_TEXT);
  
  // Volume control
  drawVolumeControl(10, 235, 300, AudioPlayer.getVolume());
  
  // Back button
  drawBackButton();
  
  // Store buttons for touch handling
  buttons.clear();
  Button playBtn = {startX, btnY, btnSize, btnSize, "Play", PLAYER_ACTION_PLAY_PAUSE, true};
  Button stopBtn = {startX + btnSize + btnSpacing, btnY, btnSize, btnSize, "Stop", PLAYER_ACTION_STOP, true};
  buttons.push_back(playBtn);
  buttons.push_back(stopBtn);
}

void UIManagerClass::showSettingsScreen() {
  Display.clear();
  drawHeader("Settings");
  drawBackButton();
  
  int y = 50;
  
  Display.drawButton(40, y, 240, 40, "📶 WiFi Settings", COLOR_BUTTON, COLOR_TEXT);
  y += 50;
  
  Display.drawButton(40, y, 240, 40, "🎨 Display Settings", COLOR_BUTTON, COLOR_TEXT);
  y += 50;
  
  Display.drawButton(40, y, 240, 40, "🔊 Audio Settings", COLOR_BUTTON, COLOR_TEXT);
  y += 50;
  
  Display.drawButton(40, y, 240, 40, "ℹ️ About", COLOR_BUTTON, COLOR_TEXT);
}

void UIManagerClass::showAddMenu() {
  Display.clear();
  drawHeader("Add New");
  drawBackButton();
  
  Display.drawButton(40, 80, 240, 50, "📁 Create Folder", COLOR_PRIMARY, COLOR_TEXT);
  Display.drawButton(40, 140, 240, 50, "📻 Add Station", COLOR_SECONDARY, COLOR_TEXT);
}

void UIManagerClass::showAddStationScreen() {
  Display.clear();
  drawHeader("Add Station");
  drawBackButton();
  
  Display.drawText("Use web interface for", 20, 60, COLOR_TEXT, 1);
  Display.drawText("easier station entry:", 20, 80, COLOR_TEXT, 1);
  Display.drawText("http://" + WiFiManager.getIPAddress(), 20, 100, COLOR_PRIMARY, 1);
  
  Display.drawButton(40, 140, 240, 40, "⌨️ Use Keyboard", COLOR_BUTTON, COLOR_TEXT);
}

void UIManagerClass::showAddFolderScreen() {
  Display.clear();
  drawHeader("Create Folder");
  drawBackButton();
  
  keyboard.clear();
  keyboard.show();
}

void UIManagerClass::showDisplaySettings() {
  Display.clear();
  drawHeader("Display Settings");
  drawBackButton();
  
  Display.drawText("Brightness:", 20, 60, COLOR_TEXT, 1);
  int brightness = Display.getBrightness();
  Display.drawProgressBar(20, 80, 280, 20, (brightness * 100) / 255, COLOR_PRIMARY, COLOR_CARD_BG);
}

void UIManagerClass::showAudioSettings() {
  Display.clear();
  drawHeader("Audio Settings");
  drawBackButton();
  
  Display.drawText("Volume:", 20, 60, COLOR_TEXT, 1);
  int volume = AudioPlayer.getVolume();
  Display.drawProgressBar(20, 80, 280, 20, (volume * 100) / VOLUME_MAX, COLOR_PRIMARY, COLOR_CARD_BG);
}

void UIManagerClass::showAboutScreen() {
  Display.clear();
  drawHeader("About");
  drawBackButton();
  
  Display.drawCenteredText(APP_NAME, 60, COLOR_PRIMARY, 2);
  Display.drawCenteredText("Version " + String(APP_VERSION), 90, COLOR_TEXT_DIM, 1);
  Display.drawCenteredText("Internet Radio Boombox", 110, COLOR_TEXT_DIM, 1);
  Display.drawCenteredText("ESP32-2432S028R", 130, COLOR_TEXT_DIM, 1);
  
  Display.drawText("IP: " + WiFiManager.getIPAddress(), 20, 160, COLOR_TEXT, 1);
  Display.drawText("MAC: " + WiFiManager.getMACAddress(), 20, 180, COLOR_TEXT, 1);
}

// ============================================================================
// Screen Updates
// ============================================================================

void UIManagerClass::updateHomeScreen() {
  // Refresh if needed
}

void UIManagerClass::updateFolderView() {
  // Refresh if needed
}

void UIManagerClass::updatePlayerScreen() {
  // Update metadata display
  if (AudioPlayer.hasMetadata()) {
    Display.fillRect(0, 140, SCREEN_WIDTH, 40, COLOR_BACKGROUND);
    drawMetadata(140, AudioPlayer.getTitle(), AudioPlayer.getArtist());
  }
}

void UIManagerClass::updateSettingsScreen() {
  // Refresh if needed
}

void UIManagerClass::updateKeyboard() {
  keyboard.redraw();
}

// ============================================================================
// Touch Handling
// ============================================================================

int UIManagerClass::checkWiFiSetupTouch(TouchPoint point) {
  int btnIdx = getTouchedButton(point);
  if (btnIdx >= 0) {
    return buttons[btnIdx].action;
  }
  return WIFI_ACTION_NONE;
}

int UIManagerClass::checkHomeScreenTouch(TouchPoint point) {
  // Check settings button (top right)
  if (isPointInRect(point, 280, 5, 35, 35)) {
    return HOME_ACTION_SETTINGS;
  }
  
  // Check add button (top right, before settings)
  if (isPointInRect(point, 240, 5, 35, 35)) {
    return HOME_ACTION_ADD;
  }
  
  // Check list items
  int btnIdx = getTouchedButton(point);
  if (btnIdx >= 0) {
    if (buttons[btnIdx].action < 1000) {
      return HOME_ACTION_FOLDER;
    } else {
      return HOME_ACTION_STATION;
    }
  }
  
  return HOME_ACTION_NONE;
}

int UIManagerClass::checkPlayerTouch(TouchPoint point) {
  // Check back button
  if (isPointInRect(point, 5, 5, 35, 35)) {
    return PLAYER_ACTION_BACK;
  }
  
  // Check player buttons
  int btnIdx = getTouchedButton(point);
  if (btnIdx >= 0) {
    return buttons[btnIdx].action;
  }
  
  // Check volume slider
  if (isPointInRect(point, 10, 235, 300, 20)) {
    int volume = ((point.x - 10) * VOLUME_MAX) / 300;
    AudioPlayer.setVolume(volume);
    return PLAYER_ACTION_NONE;
  }
  
  return PLAYER_ACTION_NONE;
}

int UIManagerClass::checkSettingsTouch(TouchPoint point) {
  if (isPointInRect(point, 40, 50, 240, 40)) {
    return SETTINGS_ACTION_WIFI;
  } else if (isPointInRect(point, 40, 100, 240, 40)) {
    return SETTINGS_ACTION_DISPLAY;
  } else if (isPointInRect(point, 40, 150, 240, 40)) {
    return SETTINGS_ACTION_AUDIO;
  } else if (isPointInRect(point, 40, 200, 240, 40)) {
    return SETTINGS_ACTION_ABOUT;
  } else if (isPointInRect(point, 5, 5, 35, 35)) {
    return SETTINGS_ACTION_BACK;
  }
  
  return SETTINGS_ACTION_NONE;
}

int UIManagerClass::checkFolderViewTouch(TouchPoint point) {
  return checkHomeScreenTouch(point);
}

int UIManagerClass::checkAddMenuTouch(TouchPoint point) {
  if (isPointInRect(point, 40, 80, 240, 50)) {
    return ADD_ACTION_FOLDER;
  } else if (isPointInRect(point, 40, 140, 240, 50)) {
    return ADD_ACTION_STATION;
  } else if (isPointInRect(point, 5, 5, 35, 35)) {
    return ADD_ACTION_BACK;
  }
  
  return ADD_ACTION_NONE;
}

// ============================================================================
// Helpers
// ============================================================================

int UIManagerClass::getSelectedNetwork(TouchPoint point) {
  for (size_t i = 0; i < buttons.size(); i++) {
    if (isPointInRect(point, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h)) {
      return buttons[i].action;
    }
  }
  return -1;
}

String UIManagerClass::getCurrentSSID() {
  return currentSSID;
}

String UIManagerClass::getCurrentPassword() {
  return keyboard.getText();
}

int UIManagerClass::getSelectedFolder(TouchPoint point) {
  for (size_t i = 0; i < buttons.size(); i++) {
    if (isPointInRect(point, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h)) {
      if (buttons[i].action < 1000) {
        return buttons[i].action;
      }
    }
  }
  return -1;
}

int UIManagerClass::getSelectedStation(TouchPoint point) {
  for (size_t i = 0; i < buttons.size(); i++) {
    if (isPointInRect(point, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h)) {
      if (buttons[i].action >= 1000) {
        return buttons[i].action - 1000;
      }
    }
  }
  return -1;
}

char UIManagerClass::getKeyboardKey(TouchPoint point) {
  return keyboard.getKeyAt(point.x, point.y);
}

void UIManagerClass::submitKeyboardInput() {
  currentPassword = keyboard.getText();
  keyboard.hide();
}

void UIManagerClass::keyboardBackspace() {
  keyboard.backspace();
}

void UIManagerClass::keyboardAddChar(char c) {
  keyboard.appendChar(c);
}

// ============================================================================
// Drawing Helpers
// ============================================================================

void UIManagerClass::drawHeader(const String& title) {
  Display.fillRect(0, 0, SCREEN_WIDTH, 40, COLOR_PRIMARY);
  Display.drawCenteredText(title, 12, COLOR_TEXT, 2);
}

void UIManagerClass::drawBackButton() {
  Display.fillCircle(20, 20, 15, COLOR_BUTTON);
  Display.drawText("<", 15, 12, COLOR_TEXT, 2);
}

void UIManagerClass::drawSettingsButton() {
  Display.fillCircle(295, 20, 15, COLOR_BUTTON);
  Display.drawText("*", 290, 12, COLOR_TEXT, 2);
}

void UIManagerClass::drawAddButton() {
  Display.fillCircle(255, 20, 15, COLOR_BUTTON);
  Display.drawText("+", 249, 12, COLOR_TEXT, 2);
}

void UIManagerClass::drawStationCard(int x, int y, int w, int h, Station* station, bool pressed) {
  uint16_t bgColor = pressed ? COLOR_BUTTON_PRESS : COLOR_CARD_BG;
  
  Display.fillRoundRect(x, y, w, h, 6, bgColor);
  Display.drawRoundRect(x, y, w, h, 6, COLOR_PRIMARY);
  
  // Draw icon
  Display.drawIcon(x + 5, y + 10, "📻", COLOR_PRIMARY, 2);
  
  // Draw station name
  Display.drawText(station->name, x + 35, y + 12, COLOR_TEXT, 1);
}

void UIManagerClass::drawFolderCard(int x, int y, int w, int h, Folder* folder, bool pressed) {
  uint16_t bgColor = pressed ? COLOR_BUTTON_PRESS : COLOR_CARD_BG;
  
  Display.fillRoundRect(x, y, w, h, 6, bgColor);
  Display.drawRoundRect(x, y, w, h, 6, COLOR_SECONDARY);
  
  // Draw icon
  Display.drawIcon(x + 5, y + 10, "📁", COLOR_SECONDARY, 2);
  
  // Draw folder name
  Display.drawText(folder->name, x + 35, y + 12, COLOR_TEXT, 1);
  
  // Draw count
  String count = String(folder->stationCount) + " stations";
  Display.drawText(count, x + 35, y + 25, COLOR_TEXT_DIM, 1);
}

void UIManagerClass::drawWiFiNetwork(int x, int y, int w, const String& ssid, int rssi, bool secure) {
  Display.fillRoundRect(x, y, w, 40, 6, COLOR_CARD_BG);
  Display.drawRoundRect(x, y, w, 40, 6, COLOR_PRIMARY);
  
  // Draw SSID
  Display.drawText(ssid, x + 10, y + 12, COLOR_TEXT, 1);
  
  // Draw signal strength
  String signal = String(rssi) + " dBm";
  Display.drawText(signal, x + 10, y + 25, COLOR_TEXT_DIM, 1);
  
  // Draw lock icon if secured
  if (secure) {
    Display.drawText("🔒", x + w - 25, y + 12, COLOR_TEXT_DIM, 1);
  }
}

void UIManagerClass::drawMetadata(int y, const String& title, const String& artist) {
  // Draw title
  if (title.length() > 0) {
    Display.drawCenteredText(title, y, COLOR_TEXT, 1);
  }
  
  // Draw artist
  if (artist.length() > 0) {
    Display.drawCenteredText(artist, y + 15, COLOR_TEXT_DIM, 1);
  }
}

void UIManagerClass::drawVolumeControl(int x, int y, int w, int volume) {
  // Draw volume icon
  Display.drawIcon(x, y - 20, "🔊", COLOR_TEXT, 1);
  
  // Draw volume bar
  Display.drawProgressBar(x, y, w, 20, (volume * 100) / VOLUME_MAX, COLOR_PRIMARY, COLOR_CARD_BG);
  
  // Draw volume level text
  String volText = String(volume) + "/" + String(VOLUME_MAX);
  Display.drawText(volText, x + w + 10, y + 5, COLOR_TEXT, 1);
}

bool UIManagerClass::isPointInRect(TouchPoint point, int x, int y, int w, int h) {
  return point.x >= x && point.x < x + w && point.y >= y && point.y < y + h;
}

int UIManagerClass::getTouchedButton(TouchPoint point) {
  for (size_t i = 0; i < buttons.size(); i++) {
    if (isPointInRect(point, buttons[i].x, buttons[i].y, buttons[i].w, buttons[i].h)) {
      return i;
    }
  }
  return -1;
}

void UIManagerClass::scrollUp() {
  if (scrollPosition > 0) {
    scrollPosition--;
  }
}

void UIManagerClass::scrollDown() {
  scrollPosition++;
}

void UIManagerClass::setScrollPosition(int position) {
  scrollPosition = position;
}

int UIManagerClass::getScrollPosition() {
  return scrollPosition;
}
