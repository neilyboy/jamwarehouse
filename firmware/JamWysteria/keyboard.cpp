/**
 * On-Screen Keyboard Implementation
 */

#include "keyboard.h"

// Keyboard layouts
const char* OnScreenKeyboard::layoutLower[] = {
  "qwertyuiop",
  "asdfghjkl",
  "zxcvbnm",
  " "
};

const char* OnScreenKeyboard::layoutUpper[] = {
  "QWERTYUIOP",
  "ASDFGHJKL",
  "ZXCVBNM",
  " "
};

const char* OnScreenKeyboard::layoutNumbers[] = {
  "1234567890",
  "-/:;()$&@",
  ".,?!'\"",
  " "
};

const char* OnScreenKeyboard::layoutSymbols[] = {
  "[]{}#%^*+=",
  "_\\|~<>€£¥",
  ".,?!'\"",
  " "
};

OnScreenKeyboard::OnScreenKeyboard() :
  visible(false),
  currentMode(KEYBOARD_LOWERCASE),
  inputText(""),
  maxLength(64),
  cursorPosition(0),
  keyWidth(KEY_WIDTH),
  keyHeight(KEY_HEIGHT),
  keySpacing(KEY_SPACING),
  startX(5),
  startY(120) {
}

void OnScreenKeyboard::init() {
  Serial.println("[KEYBOARD] Initialized");
}

void OnScreenKeyboard::show() {
  visible = true;
  draw();
}

void OnScreenKeyboard::hide() {
  visible = false;
}

void OnScreenKeyboard::draw() {
  if (!visible) return;
  
  // Clear keyboard area
  Display.fillRect(0, startY - 30, SCREEN_WIDTH, SCREEN_HEIGHT - startY + 30, COLOR_BACKGROUND);
  
  // Draw input field
  drawInputField();
  
  // Draw keys
  drawKeyLayout();
}

void OnScreenKeyboard::redraw() {
  draw();
}

char OnScreenKeyboard::getKeyAt(int x, int y) {
  if (!visible) return 0;
  
  // Check if touch is in keyboard area
  if (y < startY) {
    return 0; // Touch in input field area
  }
  
  // Calculate row
  int row = (y - startY) / (keyHeight + keySpacing);
  if (row < 0 || row >= 4) return 0;
  
  // Get layout for current row
  const char* rowLayout = getCurrentLayout(row);
  if (rowLayout == nullptr) return 0;
  
  int rowLength = strlen(rowLayout);
  
  // Special handling for space bar (row 3)
  if (row == 3) {
    // Space bar spans most of the width
    int spaceX = startX + (keyWidth + keySpacing) * 2;
    int spaceW = (keyWidth + keySpacing) * 6;
    if (x >= spaceX && x < spaceX + spaceW) {
      return ' ';
    }
    
    // Backspace button
    int backX = startX + (keyWidth + keySpacing) * 8;
    if (x >= backX && x < backX + keyWidth * 2) {
      return '\b';
    }
    
    return 0;
  }
  
  // Calculate column
  int col = (x - startX) / (keyWidth + keySpacing);
  if (col < 0 || col >= rowLength) return 0;
  
  // Check for special keys
  if (row == 2 && col == 0) {
    // Shift key
    toggleShift();
    return 0;
  }
  
  if (row == 2 && col == 9) {
    // Symbols/Numbers toggle
    if (currentMode == KEYBOARD_LOWERCASE || currentMode == KEYBOARD_UPPERCASE) {
      setMode(KEYBOARD_NUMBERS);
    } else {
      setMode(KEYBOARD_LOWERCASE);
    }
    return 0;
  }
  
  // Return character at position
  if (col < rowLength) {
    return rowLayout[col];
  }
  
  return 0;
}

void OnScreenKeyboard::setMode(KeyboardMode mode) {
  currentMode = mode;
  redraw();
}

KeyboardMode OnScreenKeyboard::getMode() {
  return currentMode;
}

void OnScreenKeyboard::toggleShift() {
  if (currentMode == KEYBOARD_LOWERCASE) {
    currentMode = KEYBOARD_UPPERCASE;
  } else if (currentMode == KEYBOARD_UPPERCASE) {
    currentMode = KEYBOARD_LOWERCASE;
  }
  redraw();
}

void OnScreenKeyboard::setText(const String& text) {
  inputText = text;
  cursorPosition = text.length();
  if (visible) {
    drawInputField();
  }
}

String OnScreenKeyboard::getText() {
  return inputText;
}

void OnScreenKeyboard::appendChar(char c) {
  if (inputText.length() < maxLength) {
    inputText += c;
    cursorPosition++;
    
    // Auto-shift off after typing one character
    if (currentMode == KEYBOARD_UPPERCASE) {
      currentMode = KEYBOARD_LOWERCASE;
      redraw();
    } else if (visible) {
      drawInputField();
    }
  }
}

void OnScreenKeyboard::backspace() {
  if (inputText.length() > 0) {
    inputText.remove(inputText.length() - 1);
    cursorPosition--;
    if (visible) {
      drawInputField();
    }
  }
}

void OnScreenKeyboard::clear() {
  inputText = "";
  cursorPosition = 0;
  if (visible) {
    drawInputField();
  }
}

void OnScreenKeyboard::setMaxLength(int length) {
  maxLength = length;
}

int OnScreenKeyboard::getMaxLength() {
  return maxLength;
}

bool OnScreenKeyboard::isVisible() {
  return visible;
}

void OnScreenKeyboard::drawKey(int x, int y, int w, int h, const char* label, bool special) {
  uint16_t bgColor = special ? COLOR_BUTTON_PRESS : COLOR_BUTTON;
  uint16_t textColor = COLOR_TEXT;
  
  // Draw key background
  Display.fillRoundRect(x, y, w, h, 4, bgColor);
  Display.drawRoundRect(x, y, w, h, 4, COLOR_TEXT_DIM);
  
  // Draw label centered
  int textWidth = strlen(label) * 6; // Approximate
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - 8) / 2;
  
  Display.drawText(label, textX, textY, textColor, 1);
}

void OnScreenKeyboard::drawInputField() {
  int fieldX = 10;
  int fieldY = startY - 30;
  int fieldW = SCREEN_WIDTH - 20;
  int fieldH = 25;
  
  // Draw input field background
  Display.fillRoundRect(fieldX, fieldY, fieldW, fieldH, 4, COLOR_CARD_BG);
  Display.drawRoundRect(fieldX, fieldY, fieldW, fieldH, 4, COLOR_PRIMARY);
  
  // Draw text
  String displayText = inputText;
  if (displayText.length() > 30) {
    displayText = displayText.substring(displayText.length() - 30);
  }
  
  Display.drawText(displayText, fieldX + 5, fieldY + 8, COLOR_TEXT, 1);
  
  // Draw cursor
  int cursorX = fieldX + 5 + (displayText.length() * 6);
  if (millis() % 1000 < 500) {
    Display.drawLine(cursorX, fieldY + 5, cursorX, fieldY + fieldH - 5, COLOR_PRIMARY);
  }
}

void OnScreenKeyboard::drawKeyLayout() {
  // Draw rows 0-2 (letter keys)
  for (int row = 0; row < 3; row++) {
    const char* rowLayout = getCurrentLayout(row);
    int rowLength = strlen(rowLayout);
    
    for (int col = 0; col < rowLength; col++) {
      int x = getKeyX(col);
      int y = getKeyY(row);
      
      char key[2] = {rowLayout[col], '\0'};
      drawKey(x, y, keyWidth, keyHeight, key);
    }
  }
  
  // Draw bottom row (special keys)
  int row3Y = getKeyY(3);
  
  // Shift key
  drawKey(startX, getKeyY(2), keyWidth, keyHeight, "⇧", true);
  
  // Numbers/Symbols toggle
  const char* toggleLabel = (currentMode == KEYBOARD_LOWERCASE || currentMode == KEYBOARD_UPPERCASE) ? "123" : "ABC";
  drawKey(startX + (keyWidth + keySpacing) * 9, getKeyY(2), keyWidth, keyHeight, toggleLabel, true);
  
  // Space bar
  int spaceX = startX + (keyWidth + keySpacing) * 2;
  int spaceW = (keyWidth + keySpacing) * 6;
  drawKey(spaceX, row3Y, spaceW, keyHeight, "Space");
  
  // Backspace
  int backX = startX + (keyWidth + keySpacing) * 8;
  drawKey(backX, row3Y, keyWidth * 2, keyHeight, "←", true);
  
  // Enter/Done
  int enterX = startX;
  drawKey(enterX, row3Y, keyWidth * 2, keyHeight, "Done", true);
}

const char* OnScreenKeyboard::getCurrentLayout(int row) {
  if (row < 0 || row >= 4) return nullptr;
  
  switch (currentMode) {
    case KEYBOARD_UPPERCASE:
      return layoutUpper[row];
    case KEYBOARD_NUMBERS:
      return layoutNumbers[row];
    case KEYBOARD_SYMBOLS:
      return layoutSymbols[row];
    case KEYBOARD_LOWERCASE:
    default:
      return layoutLower[row];
  }
}

int OnScreenKeyboard::getKeyX(int col) {
  return startX + col * (keyWidth + keySpacing);
}

int OnScreenKeyboard::getKeyY(int row) {
  return startY + row * (keyHeight + keySpacing);
}

int OnScreenKeyboard::getRowKeyCount(int row) {
  const char* rowLayout = getCurrentLayout(row);
  if (rowLayout == nullptr) return 0;
  return strlen(rowLayout);
}
