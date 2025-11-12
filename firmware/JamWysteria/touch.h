/**
 * Touch Manager for Jam Wysteria
 * 
 * Handles touch screen input using XPT2046 touch controller
 */

#ifndef TOUCH_H
#define TOUCH_H

#include <XPT2046_Touchscreen.h>
#include "config.h"

class TouchManager {
public:
  TouchManager();
  
  // Initialization
  void init();
  
  // Touch handling
  void update();
  bool isTouched();
  TouchPoint getPoint();
  TouchPoint getRawPoint();
  
  // Calibration
  void calibrate();
  void setCalibration(int minX, int maxX, int minY, int maxY);
  bool isCalibrated();
  
  // Debouncing
  void setDebounceTime(unsigned long ms);
  
private:
  XPT2046_Touchscreen touchscreen;
  
  // Calibration values
  int calMinX, calMaxX, calMinY, calMaxY;
  bool calibrated;
  
  // Debouncing
  unsigned long lastTouchTime;
  unsigned long debounceTime;
  
  // Last touch point
  TouchPoint lastPoint;
  bool wasTouched;
  
  // Helper functions
  int mapX(int rawX);
  int mapY(int rawY);
};

// Global instance
extern TouchManager Touch;

#endif // TOUCH_H
