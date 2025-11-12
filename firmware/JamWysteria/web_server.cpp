/**
 * Web Server Implementation
 */

#include "web_server.h"
#include "station_manager.h"
#include "wifi_manager.h"
#include "sd_manager.h"
#include <ArduinoJson.h>

// Global instance
WebServerClass WebServer;

WebServerClass::WebServerClass() : server(nullptr), running(false) {
}

void WebServerClass::init() {
  server = new AsyncWebServer(WEB_SERVER_PORT);
  setupRoutes();
  
  Serial.println("[WEB] Web server initialized");
  Serial.printf("[WEB] Access at: http://%s\n", WiFiManager.getIPAddress().c_str());
}

void WebServerClass::begin() {
  if (server != nullptr && !running) {
    server->begin();
    running = true;
    Serial.println("[WEB] ✓ Web server started");
  }
}

void WebServerClass::stop() {
  if (server != nullptr && running) {
    server->end();
    running = false;
    Serial.println("[WEB] Web server stopped");
  }
}

bool WebServerClass::isRunning() {
  return running;
}

void WebServerClass::update() {
  // AsyncWebServer handles requests asynchronously
  // No update needed in loop
}

void WebServerClass::setupRoutes() {
  // Main pages
  server->on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleRoot(request);
  });
  
  server->on("/stations", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleStationManager(request);
  });
  
  server->on("/admin", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleAdmin(request);
  });
  
  // API endpoints - Stations
  server->on("/api/stations", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleAPIGetStations(request);
  });
  
  server->on("/api/station/add", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIAddStation(request);
  });
  
  server->on("/api/station/update", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIUpdateStation(request);
  });
  
  server->on("/api/station/delete", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIDeleteStation(request);
  });
  
  // API endpoints - Folders
  server->on("/api/folder/add", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIAddFolder(request);
  });
  
  server->on("/api/folder/delete", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIDeleteFolder(request);
  });
  
  // API endpoints - Import/Export
  server->on("/api/import", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIImport(request);
  });
  
  server->on("/api/export", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleAPIExport(request);
  });
  
  // API endpoints - Config
  server->on("/api/config", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleAPIGetConfig(request);
  });
  
  server->on("/api/config", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPISetConfig(request);
  });
  
  // API endpoints - WiFi
  server->on("/api/wifi/scan", HTTP_GET, [this](AsyncWebServerRequest* request) {
    this->handleAPIGetWiFiNetworks(request);
  });
  
  server->on("/api/wifi/set", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPISetWiFi(request);
  });
  
  // API endpoints - System
  server->on("/api/restart", HTTP_POST, [this](AsyncWebServerRequest* request) {
    this->handleAPIRestart(request);
  });
  
  // File upload handler
  server->on("/api/upload", HTTP_POST,
    [](AsyncWebServerRequest* request) {
      request->send(200, "application/json", "{\"success\":true}");
    },
    [this](AsyncWebServerRequest* request, String filename, size_t index, 
           uint8_t* data, size_t len, bool final) {
      this->handleFileUpload(request, filename, index, data, len, final);
    }
  );
  
  // 404 handler
  server->onNotFound([this](AsyncWebServerRequest* request) {
    this->handleNotFound(request);
  });
}

// ============================================================================
// Page Handlers
// ============================================================================

void WebServerClass::handleRoot(AsyncWebServerRequest* request) {
  String html = R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Jam Wysteria - Internet Radio</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: #333;
      min-height: 100vh;
      padding: 20px;
    }
    .container {
      max-width: 800px;
      margin: 0 auto;
    }
    .header {
      text-align: center;
      color: white;
      margin-bottom: 40px;
    }
    .header h1 {
      font-size: 3em;
      margin-bottom: 10px;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
    }
    .header p {
      font-size: 1.2em;
      opacity: 0.9;
    }
    .card {
      background: white;
      border-radius: 12px;
      padding: 30px;
      margin-bottom: 20px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.2);
    }
    .card h2 {
      color: #667eea;
      margin-bottom: 15px;
    }
    .btn {
      display: inline-block;
      padding: 12px 24px;
      background: #667eea;
      color: white;
      text-decoration: none;
      border-radius: 8px;
      font-weight: 600;
      transition: all 0.3s;
      border: none;
      cursor: pointer;
      margin: 5px;
    }
    .btn:hover {
      background: #5568d3;
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(102, 126, 234, 0.4);
    }
    .btn-secondary {
      background: #48bb78;
    }
    .btn-secondary:hover {
      background: #38a169;
    }
    .info-grid {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
      gap: 15px;
      margin-top: 20px;
    }
    .info-item {
      padding: 15px;
      background: #f7fafc;
      border-radius: 8px;
      border-left: 4px solid #667eea;
    }
    .info-label {
      font-size: 0.9em;
      color: #718096;
      margin-bottom: 5px;
    }
    .info-value {
      font-size: 1.2em;
      font-weight: 600;
      color: #2d3748;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>🎵 Jam Wysteria</h1>
      <p>Internet Radio Boombox</p>
    </div>
    
    <div class="card">
      <h2>Welcome!</h2>
      <p>Your ESP32 internet radio is up and running. Use the buttons below to manage your stations or configure settings.</p>
      <div style="margin-top: 20px;">
        <a href="/stations" class="btn">📻 Manage Stations</a>
        <a href="/admin" class="btn btn-secondary">⚙️ Settings & Admin</a>
      </div>
    </div>
    
    <div class="card">
      <h2>Device Information</h2>
      <div class="info-grid">
        <div class="info-item">
          <div class="info-label">Version</div>
          <div class="info-value">)" + String(APP_VERSION) + R"(</div>
        </div>
        <div class="info-item">
          <div class="info-label">IP Address</div>
          <div class="info-value" id="ipAddress">Loading...</div>
        </div>
        <div class="info-item">
          <div class="info-label">WiFi Signal</div>
          <div class="info-value" id="wifiSignal">Loading...</div>
        </div>
        <div class="info-item">
          <div class="info-label">Free Storage</div>
          <div class="info-value" id="freeStorage">Loading...</div>
        </div>
      </div>
    </div>
  </div>
  
  <script>
    // Load device info
    fetch('/api/config')
      .then(r => r.json())
      .then(data => {
        document.getElementById('ipAddress').textContent = data.ipAddress || 'N/A';
        document.getElementById('wifiSignal').textContent = (data.rssi || 'N/A') + ' dBm';
        document.getElementById('freeStorage').textContent = data.freeStorage || 'N/A';
      });
  </script>
</body>
</html>
  )";
  
  request->send(200, "text/html", html);
}

void WebServerClass::handleStationManager(AsyncWebServerRequest* request) {
  String html = generateStationManagerHTML();
  request->send(200, "text/html", html);
}

void WebServerClass::handleAdmin(AsyncWebServerRequest* request) {
  String html = generateAdminHTML();
  request->send(200, "text/html", html);
}

void WebServerClass::handleNotFound(AsyncWebServerRequest* request) {
  request->send(404, "text/plain", "404 - Not Found");
}

// ============================================================================
// API Handlers
// ============================================================================

void WebServerClass::handleAPIGetStations(AsyncWebServerRequest* request) {
  String json = StationManager.exportStations();
  request->send(200, "application/json", json);
}

void WebServerClass::handleAPIAddStation(AsyncWebServerRequest* request) {
  if (!request->hasParam("name", true) || !request->hasParam("url", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing parameters\"}");
    return;
  }
  
  String name = request->getParam("name", true)->value();
  String url = request->getParam("url", true)->value();
  String icon = request->hasParam("icon", true) ? request->getParam("icon", true)->value() : "";
  String parent = request->hasParam("parent", true) ? request->getParam("parent", true)->value() : "/";
  
  int id = StationManager.addStation(name, url, icon, parent);
  
  String response = "{\"success\":true,\"id\":" + String(id) + "}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIUpdateStation(AsyncWebServerRequest* request) {
  if (!request->hasParam("id", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing station ID\"}");
    return;
  }
  
  int id = request->getParam("id", true)->value().toInt();
  String name = request->hasParam("name", true) ? request->getParam("name", true)->value() : "";
  String url = request->hasParam("url", true) ? request->getParam("url", true)->value() : "";
  String icon = request->hasParam("icon", true) ? request->getParam("icon", true)->value() : "";
  
  bool success = StationManager.updateStation(id, name, url, icon);
  
  String response = success ? "{\"success\":true}" : "{\"success\":false,\"error\":\"Station not found\"}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIDeleteStation(AsyncWebServerRequest* request) {
  if (!request->hasParam("id", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing station ID\"}");
    return;
  }
  
  int id = request->getParam("id", true)->value().toInt();
  bool success = StationManager.removeStation(id);
  
  String response = success ? "{\"success\":true}" : "{\"success\":false,\"error\":\"Station not found\"}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIAddFolder(AsyncWebServerRequest* request) {
  if (!request->hasParam("name", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing folder name\"}");
    return;
  }
  
  String name = request->getParam("name", true)->value();
  String icon = request->hasParam("icon", true) ? request->getParam("icon", true)->value() : "";
  String parent = request->hasParam("parent", true) ? request->getParam("parent", true)->value() : "/";
  
  int id = StationManager.addFolder(name, icon, parent);
  
  String response = "{\"success\":true,\"id\":" + String(id) + "}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIDeleteFolder(AsyncWebServerRequest* request) {
  if (!request->hasParam("id", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing folder ID\"}");
    return;
  }
  
  int id = request->getParam("id", true)->value().toInt();
  bool success = StationManager.removeFolder(id);
  
  String response = success ? "{\"success\":true}" : "{\"success\":false,\"error\":\"Folder not found\"}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIImport(AsyncWebServerRequest* request) {
  if (!request->hasParam("data", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing import data\"}");
    return;
  }
  
  String data = request->getParam("data", true)->value();
  String format = request->hasParam("format", true) ? request->getParam("format", true)->value() : "json";
  
  bool success = false;
  if (format == "csv") {
    success = StationManager.importStationsCSV(data);
  } else {
    success = StationManager.importStations(data);
  }
  
  String response = success ? "{\"success\":true}" : "{\"success\":false,\"error\":\"Import failed\"}";
  request->send(200, "application/json", response);
}

void WebServerClass::handleAPIExport(AsyncWebServerRequest* request) {
  String json = StationManager.exportStations();
  request->send(200, "application/json", json);
}

void WebServerClass::handleAPIGetConfig(AsyncWebServerRequest* request) {
  DynamicJsonDocument doc(1024);
  
  doc["ipAddress"] = WiFiManager.getIPAddress();
  doc["ssid"] = WiFiManager.getSSID();
  doc["rssi"] = WiFiManager.getRSSI();
  doc["hostname"] = WiFiManager.getHostname();
  doc["version"] = APP_VERSION;
  
  uint64_t free = SDManager.getFreeBytes();
  uint64_t total = SDManager.getTotalBytes();
  doc["freeStorage"] = String(free / (1024 * 1024)) + " MB";
  doc["totalStorage"] = String(total / (1024 * 1024)) + " MB";
  
  doc["volume"] = SDManager.getConfig().volume;
  doc["brightness"] = SDManager.getConfig().brightness;
  
  String json;
  serializeJson(doc, json);
  
  request->send(200, "application/json", json);
}

void WebServerClass::handleAPISetConfig(AsyncWebServerRequest* request) {
  AppConfig config = SDManager.getConfig();
  
  if (request->hasParam("volume", true)) {
    config.volume = request->getParam("volume", true)->value().toInt();
  }
  
  if (request->hasParam("brightness", true)) {
    config.brightness = request->getParam("brightness", true)->value().toInt();
  }
  
  SDManager.setConfig(config);
  
  request->send(200, "application/json", "{\"success\":true}");
}

void WebServerClass::handleAPIGetWiFiNetworks(AsyncWebServerRequest* request) {
  int count = WiFiManager.scanNetworks();
  
  DynamicJsonDocument doc(2048);
  JsonArray networks = doc.createNestedArray("networks");
  
  for (int i = 0; i < count && i < WIFI_SCAN_MAX_NETWORKS; i++) {
    JsonObject network = networks.createNestedObject();
    network["ssid"] = WiFiManager.getScannedSSID(i);
    network["rssi"] = WiFiManager.getScannedRSSI(i);
    network["secure"] = WiFiManager.isScannedSecure(i);
  }
  
  String json;
  serializeJson(doc, json);
  
  request->send(200, "application/json", json);
}

void WebServerClass::handleAPISetWiFi(AsyncWebServerRequest* request) {
  if (!request->hasParam("ssid", true) || !request->hasParam("password", true)) {
    request->send(400, "application/json", "{\"error\":\"Missing WiFi credentials\"}");
    return;
  }
  
  String ssid = request->getParam("ssid", true)->value();
  String password = request->getParam("password", true)->value();
  
  WiFiManager.saveCredentials(ssid, password);
  
  request->send(200, "application/json", "{\"success\":true,\"message\":\"WiFi settings saved. Device will reconnect.\"}");
  
  // Reconnect in 2 seconds
  delay(2000);
  WiFiManager.disconnect();
  WiFiManager.connect();
}

void WebServerClass::handleAPIRestart(AsyncWebServerRequest* request) {
  request->send(200, "application/json", "{\"success\":true,\"message\":\"Device restarting...\"}");
  
  delay(1000);
  ESP.restart();
}

void WebServerClass::handleFileUpload(AsyncWebServerRequest* request, String filename, 
                                      size_t index, uint8_t* data, size_t len, bool final) {
  static File uploadFile;
  
  if (index == 0) {
    Serial.printf("[WEB] Upload started: %s\n", filename.c_str());
    String path = "/logos/" + filename;
    uploadFile = SD.open(path.c_str(), FILE_WRITE);
  }
  
  if (uploadFile) {
    uploadFile.write(data, len);
  }
  
  if (final) {
    if (uploadFile) {
      uploadFile.close();
    }
    Serial.printf("[WEB] Upload complete: %s (%d bytes)\n", filename.c_str(), index + len);
  }
}

// ============================================================================
// HTML Generators
// ============================================================================

String WebServerClass::generateStationManagerHTML() {
  // This would be a large HTML file with station management interface
  // For brevity, returning a simplified version
  return R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Station Manager - Jam Wysteria</title>
  <style>
    /* Styles similar to main page */
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: #f7fafc;
      padding: 20px;
    }
    .container { max-width: 1200px; margin: 0 auto; }
    .header {
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      padding: 30px;
      border-radius: 12px;
      margin-bottom: 20px;
    }
    .btn {
      padding: 10px 20px;
      background: #667eea;
      color: white;
      border: none;
      border-radius: 6px;
      cursor: pointer;
      margin: 5px;
    }
    .station-list { margin-top: 20px; }
    .station-item {
      background: white;
      padding: 15px;
      margin: 10px 0;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>📻 Station Manager</h1>
      <p>Manage your radio stations and folders</p>
    </div>
    
    <div>
      <button class="btn" onclick="addStation()">➕ Add Station</button>
      <button class="btn" onclick="addFolder()">📁 Add Folder</button>
      <button class="btn" onclick="importStations()">📥 Import</button>
      <button class="btn" onclick="exportStations()">📤 Export</button>
      <a href="/" class="btn">🏠 Home</a>
    </div>
    
    <div class="station-list" id="stationList">
      Loading stations...
    </div>
  </div>
  
  <script>
    function loadStations() {
      fetch('/api/stations')
        .then(r => r.json())
        .then(data => {
          const list = document.getElementById('stationList');
          list.innerHTML = '';
          
          if (data.stations && data.stations.length > 0) {
            data.stations.forEach(station => {
              const item = document.createElement('div');
              item.className = 'station-item';
              item.innerHTML = `
                <strong>${station.name}</strong><br>
                <small>${station.url}</small><br>
                <button onclick="deleteStation(${station.id})">Delete</button>
              `;
              list.appendChild(item);
            });
          } else {
            list.innerHTML = '<p>No stations yet. Click "Add Station" to get started!</p>';
          }
        });
    }
    
    function addStation() {
      const name = prompt('Station name:');
      if (!name) return;
      
      const url = prompt('Stream URL:');
      if (!url) return;
      
      const formData = new FormData();
      formData.append('name', name);
      formData.append('url', url);
      
      fetch('/api/station/add', {
        method: 'POST',
        body: formData
      })
      .then(r => r.json())
      .then(data => {
        if (data.success) {
          alert('Station added!');
          loadStations();
        }
      });
    }
    
    function deleteStation(id) {
      if (!confirm('Delete this station?')) return;
      
      const formData = new FormData();
      formData.append('id', id);
      
      fetch('/api/station/delete', {
        method: 'POST',
        body: formData
      })
      .then(r => r.json())
      .then(data => {
        if (data.success) {
          loadStations();
        }
      });
    }
    
    function exportStations() {
      fetch('/api/export')
        .then(r => r.json())
        .then(data => {
          const blob = new Blob([JSON.stringify(data, null, 2)], {type: 'application/json'});
          const url = URL.createObjectURL(blob);
          const a = document.createElement('a');
          a.href = url;
          a.download = 'jam-wysteria-stations.json';
          a.click();
        });
    }
    
    loadStations();
  </script>
</body>
</html>
  )";
}

String WebServerClass::generateAdminHTML() {
  return R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Admin - Jam Wysteria</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: #f7fafc;
      padding: 20px;
    }
    .container { max-width: 800px; margin: 0 auto; }
    .header {
      background: linear-gradient(135deg, #48bb78 0%, #38a169 100%);
      color: white;
      padding: 30px;
      border-radius: 12px;
      margin-bottom: 20px;
    }
    .card {
      background: white;
      padding: 20px;
      margin: 15px 0;
      border-radius: 8px;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
    }
    .btn {
      padding: 10px 20px;
      background: #48bb78;
      color: white;
      border: none;
      border-radius: 6px;
      cursor: pointer;
      margin: 5px;
    }
    .btn-danger {
      background: #f56565;
    }
  </style>
</head>
<body>
  <div class="container">
    <div class="header">
      <h1>⚙️ Admin Panel</h1>
      <p>System configuration and management</p>
    </div>
    
    <div class="card">
      <h2>WiFi Settings</h2>
      <button class="btn" onclick="scanWiFi()">🔍 Scan Networks</button>
      <div id="wifiNetworks"></div>
    </div>
    
    <div class="card">
      <h2>System Actions</h2>
      <button class="btn btn-danger" onclick="restart()">🔄 Restart Device</button>
      <a href="/" class="btn">🏠 Home</a>
    </div>
  </div>
  
  <script>
    function scanWiFi() {
      document.getElementById('wifiNetworks').innerHTML = 'Scanning...';
      fetch('/api/wifi/scan')
        .then(r => r.json())
        .then(data => {
          let html = '<ul>';
          data.networks.forEach(network => {
            html += `<li>${network.ssid} (${network.rssi} dBm) ${network.secure ? '🔒' : ''}</li>`;
          });
          html += '</ul>';
          document.getElementById('wifiNetworks').innerHTML = html;
        });
    }
    
    function restart() {
      if (!confirm('Restart device?')) return;
      fetch('/api/restart', { method: 'POST' })
        .then(() => {
          alert('Device restarting...');
        });
    }
  </script>
</body>
</html>
  )";
}
