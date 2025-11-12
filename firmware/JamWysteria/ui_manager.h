/**
 * UI Manager for Jam Wysteria
 * 
 * Manages all user interface screens and interactions
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "config.h"
#include "display.h"
#include "keyboard.h"
#include <vector>

// Button structure
struct Button {
  int x, y, w, h;
  String label;
  int action;
  bool visible;
};

class UIManagerClass {
public:
  UIManagerClass();
  
  // Initialization
  void init();
  
  // Screen rendering
  void showWiFiSetupScreen();
  void showWiFiNetworkList();
  void showPasswordEntry(const String& ssid);
  void showHomeScreen();
  void showFolderView();
  void showPlayerScreen(Station* station);
  void showSettingsScreen();
  void showAddMenu();
  void showAddStationScreen();
  void showAddFolderScreen();
  void showDisplaySettings();
  void showAudioSettings();
  void showAboutScreen();
  
  // Screen updates
  void updateHomeScreen();
  void updateFolderView();
  void updatePlayerScreen();
  void updateSettingsScreen();
  void updateKeyboard();
  
  // Touch handling
  int checkWiFiSetupTouch(TouchPoint point);
  int checkHomeScreenTouch(TouchPoint point);
  int checkPlayerTouch(TouchPoint point);
  int checkSettingsTouch(TouchPoint point);
  int checkFolderViewTouch(TouchPoint point);
  int checkAddMenuTouch(TouchPoint point);
  
  // WiFi setup helpers
  int getSelectedNetwork(TouchPoint point);
  String getCurrentSSID();
  String getCurrentPassword();
  
  // List navigation helpers
  int getSelectedFolder(TouchPoint point);
  int getSelectedStation(TouchPoint point);
  
  // Keyboard helpers
  char getKeyboardKey(TouchPoint point);
  void submitKeyboardInput();
  void keyboardBackspace();
  void keyboardAddChar(char c);
  
  // Scrolling
  void scrollUp();
  void scrollDown();
  void setScrollPosition(int position);
  int getScrollPosition();
  
private:
  // Current screen state
  String currentSSID;
  String currentPassword;
  int scrollPosition;
  int selectedIndex;
  
  // Buttons
  std::vector<Button> buttons;
  
  // Keyboard
  OnScreenKeyboard keyboard;
  
  // Drawing helpers
  void drawHeader(const String& title);
  void drawBackButton();
  void drawSettingsButton();
  void drawAddButton();
  void drawList(std::vector<String> items, std::vector<String> icons, int startY);
  void drawStationCard(int x, int y, int w, int h, Station* station, bool pressed = false);
  void drawFolderCard(int x, int y, int w, int h, Folder* folder, bool pressed = false);
  void drawWiFiNetwork(int x, int y, int w, const String& ssid, int rssi, bool secure);
  void drawMetadata(int y, const String& title, const String& artist);
  void drawVolumeControl(int x, int y, int w, int volume);
  
  // Touch helpers
  bool isPointInRect(TouchPoint point, int x, int y, int w, int h);
  int getTouchedButton(TouchPoint point);
};

// Global instance
extern UIManagerClass UIManager;

#endif // UI_MANAGER_H
