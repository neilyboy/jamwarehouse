/**
 * Web Server for Jam Wysteria
 * 
 * Provides web interface for station management and configuration
 */

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include "config.h"

class WebServerClass {
public:
  WebServerClass();
  
  // Initialization
  void init();
  void begin();
  void stop();
  
  // Status
  bool isRunning();
  
  // Update (call in loop)
  void update();
  
private:
  AsyncWebServer* server;
  bool running;
  
  // Route handlers
  void setupRoutes();
  
  // Page handlers
  void handleRoot(AsyncWebServerRequest* request);
  void handleStationManager(AsyncWebServerRequest* request);
  void handleAdmin(AsyncWebServerRequest* request);
  void handleNotFound(AsyncWebServerRequest* request);
  
  // API handlers
  void handleAPIGetStations(AsyncWebServerRequest* request);
  void handleAPIAddStation(AsyncWebServerRequest* request);
  void handleAPIUpdateStation(AsyncWebServerRequest* request);
  void handleAPIDeleteStation(AsyncWebServerRequest* request);
  void handleAPIAddFolder(AsyncWebServerRequest* request);
  void handleAPIDeleteFolder(AsyncWebServerRequest* request);
  void handleAPIImport(AsyncWebServerRequest* request);
  void handleAPIExport(AsyncWebServerRequest* request);
  void handleAPIGetConfig(AsyncWebServerRequest* request);
  void handleAPISetConfig(AsyncWebServerRequest* request);
  void handleAPIGetWiFiNetworks(AsyncWebServerRequest* request);
  void handleAPISetWiFi(AsyncWebServerRequest* request);
  void handleAPIRestart(AsyncWebServerRequest* request);
  
  // File upload handler
  void handleFileUpload(AsyncWebServerRequest* request, String filename, 
                       size_t index, uint8_t* data, size_t len, bool final);
  
  // Helper functions
  String getContentType(const String& filename);
  String generateHTML(const String& title, const String& content);
  String generateStationManagerHTML();
  String generateAdminHTML();
};

// Global instance
extern WebServerClass WebServer;

#endif // WEB_SERVER_H
