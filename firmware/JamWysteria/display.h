/**
 * Display Manager for Jam Wysteria
 * 
 * Handles all display operations using TFT_eSPI library
 * for the ILI9341 display on ESP32-2432S028R
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <SPI.h>
#include "config.h"

class DisplayManager {
public:
  DisplayManager();
  
  // Initialization
  void init();
  
  // Basic operations
  void clear();
  void clear(uint16_t color);
  void fillScreen(uint16_t color);
  void setBrightness(uint8_t brightness);
  uint8_t getBrightness();
  
  // Drawing primitives
  void drawPixel(int x, int y, uint16_t color);
  void drawLine(int x0, int y0, int x1, int y1, uint16_t color);
  void drawRect(int x, int y, int w, int h, uint16_t color);
  void fillRect(int x, int y, int w, int h, uint16_t color);
  void drawRoundRect(int x, int y, int w, int h, int radius, uint16_t color);
  void fillRoundRect(int x, int y, int w, int h, int radius, uint16_t color);
  void drawCircle(int x, int y, int radius, uint16_t color);
  void fillCircle(int x, int y, int radius, uint16_t color);
  
  // Text operations
  void setTextColor(uint16_t color);
  void setTextColor(uint16_t fgColor, uint16_t bgColor);
  void setTextSize(uint8_t size);
  void setFont(uint8_t font);
  void setCursor(int x, int y);
  void print(const String& text);
  void println(const String& text);
  void drawText(const String& text, int x, int y, uint16_t color, uint8_t size = 1);
  void drawCenteredText(const String& text, int y, uint16_t color, uint8_t size = 1);
  int getTextWidth(const String& text, uint8_t size = 1);
  int getTextHeight(uint8_t size = 1);
  
  // Image operations
  void drawBitmap(int x, int y, const uint8_t* bitmap, int w, int h, uint16_t color);
  void drawRGBBitmap(int x, int y, const uint16_t* bitmap, int w, int h);
  bool drawJpeg(const char* filename, int x, int y);
  bool drawPng(const char* filename, int x, int y);
  bool drawImageFromSD(const String& filename, int x, int y, int maxW, int maxH);
  
  // UI Components
  void drawButton(int x, int y, int w, int h, const String& label, uint16_t bgColor, uint16_t textColor, bool pressed = false);
  void drawIcon(int x, int y, const String& icon, uint16_t color, uint8_t size = 2);
  void drawProgressBar(int x, int y, int w, int h, int progress, uint16_t fgColor, uint16_t bgColor);
  void drawScrollbar(int x, int y, int h, int position, int total);
  
  // High-level screens
  void showSplashScreen();
  void showMessage(const String& message, int duration = 0);
  void showError(const String& title, const String& message);
  void showLoading(const String& message);
  
  // Access to underlying TFT object
  TFT_eSPI& getTFT() { return tft; }
  
private:
  TFT_eSPI tft;
  uint8_t currentBrightness;
  
  // Helper functions
  void initBacklight();
  void drawLoadingSpinner(int x, int y, int radius, uint16_t color);
};

// Global instance
extern DisplayManager Display;

#endif // DISPLAY_H
