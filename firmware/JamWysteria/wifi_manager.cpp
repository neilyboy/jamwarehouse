/**
 * WiFi Manager Implementation
 */

#include "wifi_manager.h"
#include "sd_manager.h"

// Global instance
WiFiManagerClass WiFiManager;

WiFiManagerClass::WiFiManagerClass() :
  savedSSID(""),
  savedPassword(""),
  credentialsStored(false),
  networkCount(0),
  connected(false),
  lastConnectionAttempt(0),
  reconnectAttempts(0) {
}

void WiFiManagerClass::init() {
  WiFi.mode(WIFI_STA);
  WiFi.setHostname("jam-wysteria");
  
  // Load saved credentials
  loadCredentials();
  
  Serial.println("[WIFI] Initialized");
  Serial.print("[WIFI] MAC Address: ");
  Serial.println(getMACAddress());
}

bool WiFiManagerClass::connect() {
  if (!hasCredentials()) {
    Serial.println("[WIFI] No credentials stored");
    return false;
  }
  
  return connect(savedSSID, savedPassword);
}

bool WiFiManagerClass::connect(const String& ssid, const String& password) {
  Serial.printf("[WIFI] Connecting to: %s\n", ssid.c_str());
  
  WiFi.begin(ssid.c_str(), password.c_str());
  
  if (waitForConnection(WIFI_CONNECT_TIMEOUT)) {
    connected = true;
    reconnectAttempts = 0;
    
    Serial.println("[WIFI] ✓ Connected!");
    Serial.print("[WIFI] IP Address: ");
    Serial.println(getIPAddress());
    Serial.print("[WIFI] Signal: ");
    Serial.print(getRSSI());
    Serial.println(" dBm");
    
    return true;
  } else {
    connected = false;
    Serial.println("[WIFI] ✗ Connection failed");
    return false;
  }
}

void WiFiManagerClass::disconnect() {
  WiFi.disconnect();
  connected = false;
  Serial.println("[WIFI] Disconnected");
}

bool WiFiManagerClass::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

void WiFiManagerClass::update() {
  // Check connection status
  if (credentialsStored && !isConnected() && reconnectAttempts < STREAM_MAX_RECONNECTS) {
    unsigned long now = millis();
    
    if (now - lastConnectionAttempt > WIFI_RECONNECT_DELAY) {
      Serial.println("[WIFI] Connection lost. Attempting to reconnect...");
      lastConnectionAttempt = now;
      reconnectAttempts++;
      
      if (connect()) {
        Serial.println("[WIFI] Reconnected successfully");
      } else {
        Serial.printf("[WIFI] Reconnect attempt %d/%d failed\n", 
                     reconnectAttempts, STREAM_MAX_RECONNECTS);
      }
    }
  }
}

bool WiFiManagerClass::hasCredentials() {
  return credentialsStored && savedSSID.length() > 0;
}

void WiFiManagerClass::saveCredentials(const String& ssid, const String& password) {
  savedSSID = ssid;
  savedPassword = password;
  credentialsStored = true;
  
  // Save to SD card config
  // This will be handled by SDManager
  Serial.printf("[WIFI] Credentials saved for: %s\n", ssid.c_str());
}

void WiFiManagerClass::clearCredentials() {
  savedSSID = "";
  savedPassword = "";
  credentialsStored = false;
  
  Serial.println("[WIFI] Credentials cleared");
}

String WiFiManagerClass::getSSID() {
  return savedSSID;
}

String WiFiManagerClass::getPassword() {
  return savedPassword;
}

int WiFiManagerClass::scanNetworks() {
  Serial.println("[WIFI] Scanning networks...");
  
  networkCount = WiFi.scanNetworks();
  
  Serial.printf("[WIFI] Found %d networks\n", networkCount);
  
  #ifdef DEBUG_MODE
  for (int i = 0; i < networkCount && i < WIFI_SCAN_MAX_NETWORKS; i++) {
    Serial.printf("  %d: %s (%d dBm) %s\n", 
                  i, 
                  WiFi.SSID(i).c_str(), 
                  WiFi.RSSI(i),
                  (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
  }
  #endif
  
  return networkCount;
}

int WiFiManagerClass::getNetworkCount() {
  return networkCount;
}

String WiFiManagerClass::getScannedSSID(int index) {
  if (index >= 0 && index < networkCount) {
    return WiFi.SSID(index);
  }
  return "";
}

int WiFiManagerClass::getScannedRSSI(int index) {
  if (index >= 0 && index < networkCount) {
    return WiFi.RSSI(index);
  }
  return -100;
}

bool WiFiManagerClass::isScannedSecure(int index) {
  if (index >= 0 && index < networkCount) {
    return WiFi.encryptionType(index) != WIFI_AUTH_OPEN;
  }
  return true;
}

String WiFiManagerClass::getIPAddress() {
  return WiFi.localIP().toString();
}

String WiFiManagerClass::getMACAddress() {
  return WiFi.macAddress();
}

int WiFiManagerClass::getRSSI() {
  return WiFi.RSSI();
}

String WiFiManagerClass::getHostname() {
  return WiFi.getHostname();
}

void WiFiManagerClass::setHostname(const String& hostname) {
  WiFi.setHostname(hostname.c_str());
}

void WiFiManagerClass::loadCredentials() {
  // Load credentials from SD card
  // This will be implemented when SDManager is ready
  // For now, check if stored in memory
  credentialsStored = false;
}

void WiFiManagerClass::encryptPassword(String& password) {
  // Simple XOR encryption (for basic obfuscation)
  // In production, use proper encryption
  const char key = 0xA5;
  for (size_t i = 0; i < password.length(); i++) {
    password[i] = password[i] ^ key;
  }
}

void WiFiManagerClass::decryptPassword(String& password) {
  // XOR is symmetric
  encryptPassword(password);
}

bool WiFiManagerClass::waitForConnection(int timeout) {
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    
    if (millis() - startTime > timeout) {
      return false;
    }
    
    // Print dots for progress
    if ((millis() - startTime) % 1000 == 0) {
      Serial.print(".");
    }
  }
  
  Serial.println(); // New line after dots
  return true;
}
