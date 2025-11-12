/**
 * SD Card Manager for Jam Wysteria
 * 
 * Handles SD card operations for storing configuration,
 * station data, and media files
 */

#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <SD.h>
#include <FS.h>
#include "config.h"

class SDManagerClass {
public:
  SDManagerClass();
  
  // Initialization
  bool init();
  bool isInitialized();
  
  // File operations
  bool exists(const String& path);
  bool createDir(const String& path);
  bool remove(const String& path);
  bool rename(const String& oldPath, const String& newPath);
  
  // Read operations
  String readFile(const String& path);
  bool readFile(const String& path, uint8_t* buffer, size_t length);
  size_t getFileSize(const String& path);
  
  // Write operations
  bool writeFile(const String& path, const String& content);
  bool writeFile(const String& path, const uint8_t* data, size_t length);
  bool appendFile(const String& path, const String& content);
  
  // Configuration
  bool loadConfig();
  bool saveConfig();
  AppConfig& getConfig();
  void setConfig(const AppConfig& config);
  
  // Station data
  bool loadStations(String& jsonData);
  bool saveStations(const String& jsonData);
  
  // Last played station
  void saveLastStation(const String& stationName);
  String getLastStation();
  
  // Storage info
  uint64_t getTotalBytes();
  uint64_t getUsedBytes();
  uint64_t getFreeBytes();
  
  // List directory
  std::vector<String> listDir(const String& path);
  
private:
  bool initialized;
  AppConfig config;
  
  // Helper functions
  bool ensurePathExists(const String& path);
  String getParentPath(const String& path);
};

// Global instance
extern SDManagerClass SDManager;

#endif // SD_MANAGER_H
