/**
 * WiFi Manager for Jam Wysteria
 * 
 * Handles WiFi connection, scanning, and credential management
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

class WiFiManagerClass {
public:
  WiFiManagerClass();
  
  // Initialization
  void init();
  
  // Connection management
  bool connect();
  bool connect(const String& ssid, const String& password);
  void disconnect();
  bool isConnected();
  void update();
  
  // Credential management
  bool hasCredentials();
  void saveCredentials(const String& ssid, const String& password);
  void clearCredentials();
  String getSSID();
  String getPassword();
  
  // Network scanning
  int scanNetworks();
  int getNetworkCount();
  String getScannedSSID(int index);
  int getScannedRSSI(int index);
  bool isScannedSecure(int index);
  
  // Network info
  String getIPAddress();
  String getMACAddress();
  int getRSSI();
  String getHostname();
  void setHostname(const String& hostname);
  
private:
  String savedSSID;
  String savedPassword;
  bool credentialsStored;
  
  // Scanned networks
  int networkCount;
  
  // Connection status
  bool connected;
  unsigned long lastConnectionAttempt;
  int reconnectAttempts;
  
  // Helper functions
  void loadCredentials();
  void encryptPassword(String& password);
  void decryptPassword(String& password);
  bool waitForConnection(int timeout);
};

// Global instance
extern WiFiManagerClass WiFiManager;

#endif // WIFI_MANAGER_H
