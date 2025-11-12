/**
 * On-Screen Keyboard for Jam Wysteria
 * 
 * Provides a full QWERTY keyboard with special characters
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "config.h"
#include "display.h"

enum KeyboardMode {
  KEYBOARD_LOWERCASE,
  KEYBOARD_UPPERCASE,
  KEYBOARD_NUMBERS,
  KEYBOARD_SYMBOLS
};

class OnScreenKeyboard {
public:
  OnScreenKeyboard();
  
  // Initialization
  void init();
  
  // Display
  void show();
  void hide();
  void draw();
  void redraw();
  
  // Input handling
  char getKeyAt(int x, int y);
  void setMode(KeyboardMode mode);
  KeyboardMode getMode();
  void toggleShift();
  
  // Text input
  void setText(const String& text);
  String getText();
  void appendChar(char c);
  void backspace();
  void clear();
  
  // Configuration
  void setMaxLength(int length);
  int getMaxLength();
  
  // Visibility
  bool isVisible();
  
private:
  bool visible;
  KeyboardMode currentMode;
  String inputText;
  int maxLength;
  int cursorPosition;
  
  // Keyboard layout
  static const char* layoutLower[];
  static const char* layoutUpper[];
  static const char* layoutNumbers[];
  static const char* layoutSymbols[];
  
  // Key dimensions
  int keyWidth;
  int keyHeight;
  int keySpacing;
  int startX;
  int startY;
  
  // Drawing helpers
  void drawKey(int x, int y, int w, int h, const char* label, bool special = false);
  void drawInputField();
  void drawKeyLayout();
  const char* getCurrentLayout(int row);
  int getKeyX(int col);
  int getKeyY(int row);
  int getRowKeyCount(int row);
};

#endif // KEYBOARD_H
