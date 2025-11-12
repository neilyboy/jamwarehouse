/**
 * Touch Manager Implementation
 */

#include "touch.h"

// Global instance
TouchManager Touch;

TouchManager::TouchManager() : 
  touchscreen(TOUCH_CS),
  calMinX(TOUCH_MIN_X),
  calMaxX(TOUCH_MAX_X),
  calMinY(TOUCH_MIN_Y),
  calMaxY(TOUCH_MAX_Y),
  calibrated(false),
  lastTouchTime(0),
  debounceTime(TOUCH_DEBOUNCE_MS),
  wasTouched(false) {
  
  lastPoint.x = 0;
  lastPoint.y = 0;
  lastPoint.pressed = false;
}

void TouchManager::init() {
  touchscreen.begin();
  touchscreen.setRotation(SCREEN_ROTATION);
  
  // Apply calibration values
  calibrated = true;
  
  Serial.println("[TOUCH] Initialized");
  Serial.printf("[TOUCH] Calibration: X(%d-%d), Y(%d-%d)\n", 
                calMinX, calMaxX, calMinY, calMaxY);
}

void TouchManager::update() {
  // Read touch state
  bool touched = touchscreen.touched();
  
  if (touched) {
    TS_Point p = touchscreen.getPoint();
    
    // Map to screen coordinates
    lastPoint.x = mapX(p.x);
    lastPoint.y = mapY(p.y);
    lastPoint.pressed = true;
    
    #ifdef DEBUG_MODE
    if (!wasTouched) {
      Serial.printf("[TOUCH] Pressed at (%d, %d) [Raw: %d, %d]\n", 
                    lastPoint.x, lastPoint.y, p.x, p.y);
    }
    #endif
    
    wasTouched = true;
    lastTouchTime = millis();
  } else {
    if (wasTouched) {
      #ifdef DEBUG_MODE
      Serial.println("[TOUCH] Released");
      #endif
      lastPoint.pressed = false;
      wasTouched = false;
    }
  }
}

bool TouchManager::isTouched() {
  // Check debounce time
  if (millis() - lastTouchTime < debounceTime && !wasTouched) {
    return false;
  }
  
  return lastPoint.pressed;
}

TouchPoint TouchManager::getPoint() {
  return lastPoint;
}

TouchPoint TouchManager::getRawPoint() {
  TouchPoint raw;
  
  if (touchscreen.touched()) {
    TS_Point p = touchscreen.getPoint();
    raw.x = p.x;
    raw.y = p.y;
    raw.pressed = true;
  } else {
    raw.x = 0;
    raw.y = 0;
    raw.pressed = false;
  }
  
  return raw;
}

void TouchManager::calibrate() {
  // TODO: Implement interactive calibration routine
  Serial.println("[TOUCH] Calibration routine not yet implemented");
}

void TouchManager::setCalibration(int minX, int maxX, int minY, int maxY) {
  calMinX = minX;
  calMaxX = maxX;
  calMinY = minY;
  calMaxY = maxY;
  calibrated = true;
  
  Serial.printf("[TOUCH] Calibration updated: X(%d-%d), Y(%d-%d)\n", 
                calMinX, calMaxX, calMinY, calMaxY);
}

bool TouchManager::isCalibrated() {
  return calibrated;
}

void TouchManager::setDebounceTime(unsigned long ms) {
  debounceTime = ms;
}

int TouchManager::mapX(int rawX) {
  // Map raw X coordinate to screen X
  int x = map(rawX, calMinX, calMaxX, 0, SCREEN_WIDTH);
  
  // Clamp to screen bounds
  if (x < 0) x = 0;
  if (x >= SCREEN_WIDTH) x = SCREEN_WIDTH - 1;
  
  return x;
}

int TouchManager::mapY(int rawY) {
  // Map raw Y coordinate to screen Y
  int y = map(rawY, calMinY, calMaxY, 0, SCREEN_HEIGHT);
  
  // Clamp to screen bounds
  if (y < 0) y = 0;
  if (y >= SCREEN_HEIGHT) y = SCREEN_HEIGHT - 1;
  
  return y;
}
