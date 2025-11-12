/**
 * Display Manager Implementation
 */

#include "display.h"
#include <JPEGDEC.h>
#include <PNGdec.h>

// Global instance
DisplayManager Display;

DisplayManager::DisplayManager() : tft(), currentBrightness(BACKLIGHT_DEFAULT) {
}

void DisplayManager::init() {
  // Initialize TFT display
  tft.init();
  tft.setRotation(SCREEN_ROTATION);
  tft.fillScreen(COLOR_BACKGROUND);
  
  // Initialize backlight with PWM
  initBacklight();
  setBrightness(BACKLIGHT_DEFAULT);
  
  Serial.println("[DISPLAY] Initialized");
  Serial.printf("[DISPLAY] Resolution: %dx%d\n", SCREEN_WIDTH, SCREEN_HEIGHT);
}

void DisplayManager::initBacklight() {
  // Configure PWM for backlight control
  ledcSetup(BACKLIGHT_CHANNEL, BACKLIGHT_FREQ, BACKLIGHT_RESOLUTION);
  ledcAttachPin(TFT_LED, BACKLIGHT_CHANNEL);
}

void DisplayManager::clear() {
  tft.fillScreen(COLOR_BACKGROUND);
}

void DisplayManager::clear(uint16_t color) {
  tft.fillScreen(color);
}

void DisplayManager::fillScreen(uint16_t color) {
  tft.fillScreen(color);
}

void DisplayManager::setBrightness(uint8_t brightness) {
  currentBrightness = brightness;
  ledcWrite(BACKLIGHT_CHANNEL, brightness);
}

uint8_t DisplayManager::getBrightness() {
  return currentBrightness;
}

// ============================================================================
// Drawing Primitives
// ============================================================================

void DisplayManager::drawPixel(int x, int y, uint16_t color) {
  tft.drawPixel(x, y, color);
}

void DisplayManager::drawLine(int x0, int y0, int x1, int y1, uint16_t color) {
  tft.drawLine(x0, y0, x1, y1, color);
}

void DisplayManager::drawRect(int x, int y, int w, int h, uint16_t color) {
  tft.drawRect(x, y, w, h, color);
}

void DisplayManager::fillRect(int x, int y, int w, int h, uint16_t color) {
  tft.fillRect(x, y, w, h, color);
}

void DisplayManager::drawRoundRect(int x, int y, int w, int h, int radius, uint16_t color) {
  tft.drawRoundRect(x, y, w, h, radius, color);
}

void DisplayManager::fillRoundRect(int x, int y, int w, int h, int radius, uint16_t color) {
  tft.fillRoundRect(x, y, w, h, radius, color);
}

void DisplayManager::drawCircle(int x, int y, int radius, uint16_t color) {
  tft.drawCircle(x, y, radius, color);
}

void DisplayManager::fillCircle(int x, int y, int radius, uint16_t color) {
  tft.fillCircle(x, y, radius, color);
}

// ============================================================================
// Text Operations
// ============================================================================

void DisplayManager::setTextColor(uint16_t color) {
  tft.setTextColor(color);
}

void DisplayManager::setTextColor(uint16_t fgColor, uint16_t bgColor) {
  tft.setTextColor(fgColor, bgColor);
}

void DisplayManager::setTextSize(uint8_t size) {
  tft.setTextSize(size);
}

void DisplayManager::setFont(uint8_t font) {
  tft.setTextFont(font);
}

void DisplayManager::setCursor(int x, int y) {
  tft.setCursor(x, y);
}

void DisplayManager::print(const String& text) {
  tft.print(text);
}

void DisplayManager::println(const String& text) {
  tft.println(text);
}

void DisplayManager::drawText(const String& text, int x, int y, uint16_t color, uint8_t size) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(text);
}

void DisplayManager::drawCenteredText(const String& text, int y, uint16_t color, uint8_t size) {
  tft.setTextColor(color);
  tft.setTextSize(size);
  
  int textWidth = getTextWidth(text, size);
  int x = (SCREEN_WIDTH - textWidth) / 2;
  
  tft.setCursor(x, y);
  tft.print(text);
}

int DisplayManager::getTextWidth(const String& text, uint8_t size) {
  tft.setTextSize(size);
  return tft.textWidth(text.c_str());
}

int DisplayManager::getTextHeight(uint8_t size) {
  return 8 * size; // Standard font is 8 pixels tall
}

// ============================================================================
// UI Components
// ============================================================================

void DisplayManager::drawButton(int x, int y, int w, int h, const String& label, 
                                uint16_t bgColor, uint16_t textColor, bool pressed) {
  // Draw button background
  if (pressed) {
    fillRoundRect(x + 2, y + 2, w, h, 8, bgColor);
  } else {
    fillRoundRect(x, y, w, h, 8, bgColor);
    // Draw subtle shadow
    drawRoundRect(x + 2, y + 2, w, h, 8, COLOR_BACKGROUND);
  }
  
  // Draw button border
  drawRoundRect(x, y, w, h, 8, textColor);
  
  // Draw label centered
  int textWidth = getTextWidth(label, 2);
  int textHeight = getTextHeight(2);
  int textX = x + (w - textWidth) / 2;
  int textY = y + (h - textHeight) / 2;
  
  drawText(label, textX, textY, textColor, 2);
}

void DisplayManager::drawIcon(int x, int y, const String& icon, uint16_t color, uint8_t size) {
  // Draw emoji/unicode icon
  tft.setTextColor(color);
  tft.setTextSize(size);
  tft.setCursor(x, y);
  tft.print(icon);
}

void DisplayManager::drawProgressBar(int x, int y, int w, int h, int progress, 
                                     uint16_t fgColor, uint16_t bgColor) {
  // Draw background
  fillRoundRect(x, y, w, h, 4, bgColor);
  
  // Draw progress
  int progressWidth = (w * progress) / 100;
  if (progressWidth > 0) {
    fillRoundRect(x, y, progressWidth, h, 4, fgColor);
  }
  
  // Draw border
  drawRoundRect(x, y, w, h, 4, COLOR_TEXT_DIM);
}

void DisplayManager::drawScrollbar(int x, int y, int h, int position, int total) {
  int scrollbarHeight = h;
  int thumbHeight = (h * h) / total;
  if (thumbHeight < 20) thumbHeight = 20;
  
  int thumbY = y + (position * (h - thumbHeight)) / (total - h);
  
  // Draw scrollbar background
  fillRect(x, y, 4, scrollbarHeight, COLOR_BUTTON);
  
  // Draw thumb
  fillRoundRect(x, thumbY, 4, thumbHeight, 2, COLOR_PRIMARY);
}

// ============================================================================
// Image Operations
// ============================================================================

bool DisplayManager::drawImageFromSD(const String& filename, int x, int y, int maxW, int maxH) {
  if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) {
    return drawJpeg(filename.c_str(), x, y);
  } else if (filename.endsWith(".png")) {
    return drawPng(filename.c_str(), x, y);
  }
  return false;
}

bool DisplayManager::drawJpeg(const char* filename, int x, int y) {
  // JPEG drawing implementation
  // This requires JPEGDEC library
  // Simplified version - full implementation would decode and render JPEG
  #ifdef DEBUG_MODE
  Serial.printf("[DISPLAY] Drawing JPEG: %s at (%d, %d)\n", filename, x, y);
  #endif
  
  // TODO: Implement JPEG decoding from SD card
  return true;
}

bool DisplayManager::drawPng(const char* filename, int x, int y) {
  // PNG drawing implementation
  // This requires PNGdec library
  #ifdef DEBUG_MODE
  Serial.printf("[DISPLAY] Drawing PNG: %s at (%d, %d)\n", filename, x, y);
  #endif
  
  // TODO: Implement PNG decoding from SD card
  return true;
}

// ============================================================================
// High-Level Screens
// ============================================================================

void DisplayManager::showSplashScreen() {
  clear(COLOR_BACKGROUND);
  
  // Draw app name
  drawCenteredText(APP_NAME, 80, COLOR_PRIMARY, 3);
  
  // Draw subtitle
  drawCenteredText("Internet Radio Boombox", 120, COLOR_TEXT_DIM, 1);
  
  // Draw version
  drawCenteredText("v" + String(APP_VERSION), 140, COLOR_TEXT_DIM, 1);
  
  // Draw loading animation
  for (int i = 0; i < 3; i++) {
    drawLoadingSpinner(SCREEN_WIDTH / 2, 180, 15, COLOR_PRIMARY);
    delay(200);
  }
  
  delay(SPLASH_DURATION);
}

void DisplayManager::showMessage(const String& message, int duration) {
  // Save current screen state if needed
  
  // Draw message box
  int boxW = 280;
  int boxH = 100;
  int boxX = (SCREEN_WIDTH - boxW) / 2;
  int boxY = (SCREEN_HEIGHT - boxH) / 2;
  
  // Draw semi-transparent background (simplified - just use solid color)
  fillRoundRect(boxX, boxY, boxW, boxH, 10, COLOR_CARD_BG);
  drawRoundRect(boxX, boxY, boxW, boxH, 10, COLOR_PRIMARY);
  
  // Draw message
  drawCenteredText(message, boxY + 40, COLOR_TEXT, 2);
  
  if (duration > 0) {
    delay(duration);
  }
}

void DisplayManager::showError(const String& title, const String& message) {
  clear(COLOR_BACKGROUND);
  
  // Draw error icon
  fillCircle(SCREEN_WIDTH / 2, 60, 30, COLOR_ERROR);
  drawText("✖", SCREEN_WIDTH / 2 - 12, 45, COLOR_TEXT, 3);
  
  // Draw title
  drawCenteredText(title, 110, COLOR_ERROR, 2);
  
  // Draw message (word wrap if needed)
  int yPos = 140;
  int maxChars = 30;
  String remaining = message;
  
  while (remaining.length() > 0) {
    String line;
    if (remaining.length() <= maxChars) {
      line = remaining;
      remaining = "";
    } else {
      int spacePos = remaining.lastIndexOf(' ', maxChars);
      if (spacePos > 0) {
        line = remaining.substring(0, spacePos);
        remaining = remaining.substring(spacePos + 1);
      } else {
        line = remaining.substring(0, maxChars);
        remaining = remaining.substring(maxChars);
      }
    }
    
    drawCenteredText(line, yPos, COLOR_TEXT_DIM, 1);
    yPos += 20;
  }
}

void DisplayManager::showLoading(const String& message) {
  clear(COLOR_BACKGROUND);
  
  // Draw loading spinner
  drawLoadingSpinner(SCREEN_WIDTH / 2, 100, 20, COLOR_PRIMARY);
  
  // Draw message
  drawCenteredText(message, 150, COLOR_TEXT, 2);
}

void DisplayManager::drawLoadingSpinner(int x, int y, int radius, uint16_t color) {
  // Simple animated spinner
  static int angle = 0;
  
  for (int i = 0; i < 8; i++) {
    float a = (angle + i * 45) * 0.017453; // Convert to radians
    int x1 = x + cos(a) * radius * 0.5;
    int y1 = y + sin(a) * radius * 0.5;
    int x2 = x + cos(a) * radius;
    int y2 = y + sin(a) * radius;
    
    uint16_t c = color;
    if (i > 4) {
      // Fade older segments
      c = COLOR_TEXT_DIM;
    }
    
    drawLine(x1, y1, x2, y2, c);
  }
  
  angle += 45;
  if (angle >= 360) angle = 0;
}
