/**
 * SD Card Manager Implementation
 */

#include "sd_manager.h"
#include <ArduinoJson.h>

// Global instance
SDManagerClass SDManager;

SDManagerClass::SDManagerClass() : initialized(false) {
  // Initialize config with defaults
  config.wifiSSID = "";
  config.wifiPassword = "";
  config.volume = VOLUME_DEFAULT;
  config.brightness = BACKLIGHT_DEFAULT;
  config.lastStation = "";
  config.autoConnect = true;
  config.screenTimeout = 0; // 0 = never timeout
}

bool SDManagerClass::init() {
  Serial.println("[SD] Initializing SD card...");
  
  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial.println("[SD] ✗ SD card initialization failed");
    initialized = false;
    return false;
  }
  
  uint8_t cardType = SD.cardType();
  
  if (cardType == CARD_NONE) {
    Serial.println("[SD] ✗ No SD card detected");
    initialized = false;
    return false;
  }
  
  // Print card info
  Serial.print("[SD] Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }
  
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("[SD] Size: %lluMB\n", cardSize);
  Serial.printf("[SD] Total: %lluMB\n", getTotalBytes() / (1024 * 1024));
  Serial.printf("[SD] Used: %lluMB\n", getUsedBytes() / (1024 * 1024));
  Serial.printf("[SD] Free: %lluMB\n", getFreeBytes() / (1024 * 1024));
  
  // Create default directories
  createDir("/config");
  createDir("/logos");
  createDir("/icons");
  
  initialized = true;
  Serial.println("[SD] ✓ Initialized");
  
  return true;
}

bool SDManagerClass::isInitialized() {
  return initialized;
}

bool SDManagerClass::exists(const String& path) {
  return SD.exists(path.c_str());
}

bool SDManagerClass::createDir(const String& path) {
  if (exists(path)) {
    return true;
  }
  
  return SD.mkdir(path.c_str());
}

bool SDManagerClass::remove(const String& path) {
  return SD.remove(path.c_str());
}

bool SDManagerClass::rename(const String& oldPath, const String& newPath) {
  return SD.rename(oldPath.c_str(), newPath.c_str());
}

String SDManagerClass::readFile(const String& path) {
  if (!exists(path)) {
    Serial.printf("[SD] File not found: %s\n", path.c_str());
    return "";
  }
  
  File file = SD.open(path.c_str(), FILE_READ);
  if (!file) {
    Serial.printf("[SD] Failed to open file: %s\n", path.c_str());
    return "";
  }
  
  String content;
  while (file.available()) {
    content += (char)file.read();
  }
  
  file.close();
  return content;
}

bool SDManagerClass::readFile(const String& path, uint8_t* buffer, size_t length) {
  if (!exists(path)) {
    return false;
  }
  
  File file = SD.open(path.c_str(), FILE_READ);
  if (!file) {
    return false;
  }
  
  size_t bytesRead = file.read(buffer, length);
  file.close();
  
  return bytesRead == length;
}

size_t SDManagerClass::getFileSize(const String& path) {
  if (!exists(path)) {
    return 0;
  }
  
  File file = SD.open(path.c_str(), FILE_READ);
  if (!file) {
    return 0;
  }
  
  size_t size = file.size();
  file.close();
  
  return size;
}

bool SDManagerClass::writeFile(const String& path, const String& content) {
  // Ensure parent directory exists
  ensurePathExists(path);
  
  File file = SD.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.printf("[SD] Failed to create file: %s\n", path.c_str());
    return false;
  }
  
  size_t written = file.print(content);
  file.close();
  
  return written == content.length();
}

bool SDManagerClass::writeFile(const String& path, const uint8_t* data, size_t length) {
  // Ensure parent directory exists
  ensurePathExists(path);
  
  File file = SD.open(path.c_str(), FILE_WRITE);
  if (!file) {
    return false;
  }
  
  size_t written = file.write(data, length);
  file.close();
  
  return written == length;
}

bool SDManagerClass::appendFile(const String& path, const String& content) {
  File file = SD.open(path.c_str(), FILE_APPEND);
  if (!file) {
    return false;
  }
  
  size_t written = file.print(content);
  file.close();
  
  return written == content.length();
}

bool SDManagerClass::loadConfig() {
  Serial.println("[SD] Loading configuration...");
  
  String configData = readFile(SD_CONFIG_FILE);
  
  if (configData.length() == 0) {
    Serial.println("[SD] No configuration file found, using defaults");
    return false;
  }
  
  // Parse JSON
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, configData);
  
  if (error) {
    Serial.println("[SD] Failed to parse configuration");
    return false;
  }
  
  // Load config values
  config.wifiSSID = doc["wifi_ssid"] | "";
  config.wifiPassword = doc["wifi_password"] | "";
  config.volume = doc["volume"] | VOLUME_DEFAULT;
  config.brightness = doc["brightness"] | BACKLIGHT_DEFAULT;
  config.lastStation = doc["last_station"] | "";
  config.autoConnect = doc["auto_connect"] | true;
  config.screenTimeout = doc["screen_timeout"] | 0;
  
  Serial.println("[SD] ✓ Configuration loaded");
  return true;
}

bool SDManagerClass::saveConfig() {
  Serial.println("[SD] Saving configuration...");
  
  // Create JSON document
  DynamicJsonDocument doc(1024);
  
  doc["wifi_ssid"] = config.wifiSSID;
  doc["wifi_password"] = config.wifiPassword;
  doc["volume"] = config.volume;
  doc["brightness"] = config.brightness;
  doc["last_station"] = config.lastStation;
  doc["auto_connect"] = config.autoConnect;
  doc["screen_timeout"] = config.screenTimeout;
  
  // Serialize to string
  String configData;
  serializeJsonPretty(doc, configData);
  
  // Write to SD card
  if (writeFile(SD_CONFIG_FILE, configData)) {
    Serial.println("[SD] ✓ Configuration saved");
    return true;
  } else {
    Serial.println("[SD] ✗ Failed to save configuration");
    return false;
  }
}

AppConfig& SDManagerClass::getConfig() {
  return config;
}

void SDManagerClass::setConfig(const AppConfig& newConfig) {
  config = newConfig;
  saveConfig();
}

bool SDManagerClass::loadStations(String& jsonData) {
  jsonData = readFile(SD_STATIONS_FILE);
  return jsonData.length() > 0;
}

bool SDManagerClass::saveStations(const String& jsonData) {
  return writeFile(SD_STATIONS_FILE, jsonData);
}

void SDManagerClass::saveLastStation(const String& stationName) {
  config.lastStation = stationName;
  saveConfig();
}

String SDManagerClass::getLastStation() {
  return config.lastStation;
}

uint64_t SDManagerClass::getTotalBytes() {
  return SD.totalBytes();
}

uint64_t SDManagerClass::getUsedBytes() {
  return SD.usedBytes();
}

uint64_t SDManagerClass::getFreeBytes() {
  return getTotalBytes() - getUsedBytes();
}

std::vector<String> SDManagerClass::listDir(const String& path) {
  std::vector<String> files;
  
  File dir = SD.open(path.c_str());
  if (!dir || !dir.isDirectory()) {
    return files;
  }
  
  File file = dir.openNextFile();
  while (file) {
    files.push_back(String(file.name()));
    file = dir.openNextFile();
  }
  
  return files;
}

bool SDManagerClass::ensurePathExists(const String& path) {
  String parentPath = getParentPath(path);
  
  if (parentPath.length() > 0 && !exists(parentPath)) {
    return createDir(parentPath);
  }
  
  return true;
}

String SDManagerClass::getParentPath(const String& path) {
  int lastSlash = path.lastIndexOf('/');
  if (lastSlash > 0) {
    return path.substring(0, lastSlash);
  }
  return "";
}
