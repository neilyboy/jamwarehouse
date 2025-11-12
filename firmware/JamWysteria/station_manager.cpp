/**
 * Station Manager Implementation
 */

#include "station_manager.h"
#include "sd_manager.h"
#include <ArduinoJson.h>

// Global instance
StationManagerClass StationManager;

StationManagerClass::StationManagerClass() :
  currentFolder("/"),
  nextStationId(1),
  nextFolderId(1) {
}

void StationManagerClass::init() {
  Serial.println("[STATION] Initialized");
}

bool StationManagerClass::loadStations() {
  // Load stations from SD card
  // This will be implemented with SDManager
  Serial.println("[STATION] Loading stations from SD card...");
  
  // TODO: Load from SD_STATIONS_FILE
  // For now, return true (empty station list)
  
  Serial.printf("[STATION] Loaded %d stations and %d folders\n", 
                stations.size(), folders.size());
  return true;
}

bool StationManagerClass::saveStations() {
  Serial.println("[STATION] Saving stations to SD card...");
  
  // Create JSON document
  DynamicJsonDocument doc(8192);
  
  // Save folders
  JsonArray foldersArray = doc.createNestedArray("folders");
  for (const auto& folder : folders) {
    JsonObject folderObj = foldersArray.createNestedObject();
    folderObj["id"] = folder.id;
    folderObj["name"] = folder.name;
    folderObj["iconPath"] = folder.iconPath;
    folderObj["parentFolder"] = folder.parentFolder;
  }
  
  // Save stations
  JsonArray stationsArray = doc.createNestedArray("stations");
  for (const auto& station : stations) {
    JsonObject stationObj = stationsArray.createNestedObject();
    stationObj["id"] = station.id;
    stationObj["name"] = station.name;
    stationObj["url"] = station.url;
    stationObj["iconPath"] = station.iconPath;
    stationObj["parentFolder"] = station.parentFolder;
  }
  
  // Serialize to string
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Save to SD card (will be implemented with SDManager)
  // SDManager.writeFile(SD_STATIONS_FILE, jsonString);
  
  Serial.println("[STATION] ✓ Stations saved");
  return true;
}

int StationManagerClass::addStation(const String& name, const String& url, 
                                    const String& iconPath, const String& parentFolder) {
  Station newStation;
  newStation.id = nextStationId++;
  newStation.name = name;
  newStation.url = url;
  newStation.iconPath = iconPath.length() > 0 ? iconPath : DEFAULT_STATION_ICON;
  newStation.parentFolder = parentFolder;
  
  stations.push_back(newStation);
  
  // Update parent folder count
  updateFolderCounts(parentFolder);
  
  // Save to SD
  saveStations();
  
  Serial.printf("[STATION] Added: %s (ID: %d)\n", name.c_str(), newStation.id);
  return newStation.id;
}

bool StationManagerClass::removeStation(int id) {
  for (auto it = stations.begin(); it != stations.end(); ++it) {
    if (it->id == id) {
      String parentFolder = it->parentFolder;
      Serial.printf("[STATION] Removing: %s (ID: %d)\n", it->name.c_str(), id);
      
      stations.erase(it);
      
      // Update parent folder count
      updateFolderCounts(parentFolder);
      
      // Save to SD
      saveStations();
      
      return true;
    }
  }
  
  return false;
}

bool StationManagerClass::updateStation(int id, const String& name, 
                                       const String& url, const String& iconPath) {
  Station* station = getStation(id);
  if (station != nullptr) {
    station->name = name;
    station->url = url;
    if (iconPath.length() > 0) {
      station->iconPath = iconPath;
    }
    
    // Save to SD
    saveStations();
    
    Serial.printf("[STATION] Updated: %s (ID: %d)\n", name.c_str(), id);
    return true;
  }
  
  return false;
}

Station* StationManagerClass::getStation(int id) {
  for (auto& station : stations) {
    if (station.id == id) {
      return &station;
    }
  }
  return nullptr;
}

Station* StationManagerClass::getStationByIndex(int index) {
  auto currentStations = getCurrentStations();
  if (index >= 0 && index < currentStations.size()) {
    return currentStations[index];
  }
  return nullptr;
}

int StationManagerClass::getStationCount() {
  return getCurrentStations().size();
}

int StationManagerClass::addFolder(const String& name, const String& iconPath, 
                                   const String& parentFolder) {
  Folder newFolder;
  newFolder.id = nextFolderId++;
  newFolder.name = name;
  newFolder.iconPath = iconPath.length() > 0 ? iconPath : DEFAULT_FOLDER_ICON;
  newFolder.parentFolder = parentFolder;
  newFolder.stationCount = 0;
  newFolder.folderCount = 0;
  
  folders.push_back(newFolder);
  
  // Update parent folder count
  updateFolderCounts(parentFolder);
  
  // Save to SD
  saveStations();
  
  Serial.printf("[STATION] Added folder: %s (ID: %d)\n", name.c_str(), newFolder.id);
  return newFolder.id;
}

bool StationManagerClass::removeFolder(int id) {
  for (auto it = folders.begin(); it != folders.end(); ++it) {
    if (it->id == id) {
      String folderPath = getFullPath(it->parentFolder, it->name);
      String parentFolder = it->parentFolder;
      
      Serial.printf("[STATION] Removing folder: %s (ID: %d)\n", it->name.c_str(), id);
      
      // Remove all stations in this folder
      auto stationsToRemove = getStationsInFolder(folderPath);
      for (auto station : stationsToRemove) {
        removeStation(station->id);
      }
      
      // Remove all subfolders
      auto foldersToRemove = getFoldersInFolder(folderPath);
      for (auto subfolder : foldersToRemove) {
        removeFolder(subfolder->id);
      }
      
      folders.erase(it);
      
      // Update parent folder count
      updateFolderCounts(parentFolder);
      
      // Save to SD
      saveStations();
      
      return true;
    }
  }
  
  return false;
}

bool StationManagerClass::updateFolder(int id, const String& name, const String& iconPath) {
  Folder* folder = getFolder(id);
  if (folder != nullptr) {
    folder->name = name;
    if (iconPath.length() > 0) {
      folder->iconPath = iconPath;
    }
    
    // Save to SD
    saveStations();
    
    Serial.printf("[STATION] Updated folder: %s (ID: %d)\n", name.c_str(), id);
    return true;
  }
  
  return false;
}

Folder* StationManagerClass::getFolder(int id) {
  for (auto& folder : folders) {
    if (folder.id == id) {
      return &folder;
    }
  }
  return nullptr;
}

Folder* StationManagerClass::getFolderByIndex(int index) {
  auto currentFolders = getCurrentFolders();
  if (index >= 0 && index < currentFolders.size()) {
    return currentFolders[index];
  }
  return nullptr;
}

int StationManagerClass::getFolderCount() {
  return getCurrentFolders().size();
}

void StationManagerClass::enterFolder(int folderId) {
  Folder* folder = getFolder(folderId);
  if (folder != nullptr) {
    String newPath = getFullPath(currentFolder, folder->name);
    navigationStack.push_back(currentFolder);
    currentFolder = newPath;
    
    Serial.printf("[STATION] Entered folder: %s\n", currentFolder.c_str());
  }
}

void StationManagerClass::enterFolder(const String& folderPath) {
  if (isValidPath(folderPath)) {
    navigationStack.push_back(currentFolder);
    currentFolder = folderPath;
    
    Serial.printf("[STATION] Entered folder: %s\n", currentFolder.c_str());
  }
}

void StationManagerClass::goBack() {
  if (canGoBack()) {
    currentFolder = navigationStack.back();
    navigationStack.pop_back();
    
    Serial.printf("[STATION] Back to: %s\n", currentFolder.c_str());
  }
}

bool StationManagerClass::canGoBack() {
  return !navigationStack.empty();
}

String StationManagerClass::getCurrentPath() {
  return currentFolder;
}

String StationManagerClass::getCurrentFolderName() {
  if (currentFolder == "/") {
    return "Home";
  }
  
  int lastSlash = currentFolder.lastIndexOf('/');
  if (lastSlash >= 0) {
    return currentFolder.substring(lastSlash + 1);
  }
  
  return currentFolder;
}

std::vector<Folder*> StationManagerClass::getCurrentFolders() {
  return getFoldersInFolder(currentFolder);
}

std::vector<Station*> StationManagerClass::getCurrentStations() {
  return getStationsInFolder(currentFolder);
}

std::vector<Station*> StationManagerClass::searchStations(const String& query) {
  std::vector<Station*> results;
  
  String lowerQuery = query;
  lowerQuery.toLowerCase();
  
  for (auto& station : stations) {
    String lowerName = station.name;
    lowerName.toLowerCase();
    
    if (lowerName.indexOf(lowerQuery) >= 0) {
      results.push_back(&station);
    }
  }
  
  return results;
}

bool StationManagerClass::importStations(const String& jsonData) {
  // Parse JSON and import stations
  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, jsonData);
  
  if (error) {
    Serial.println("[STATION] JSON parse error");
    return false;
  }
  
  // Import folders
  if (doc.containsKey("folders")) {
    JsonArray foldersArray = doc["folders"];
    for (JsonObject folderObj : foldersArray) {
      addFolder(
        folderObj["name"].as<String>(),
        folderObj["iconPath"].as<String>(),
        folderObj["parent"].as<String>()
      );
    }
  }
  
  // Import stations
  if (doc.containsKey("stations")) {
    JsonArray stationsArray = doc["stations"];
    for (JsonObject stationObj : stationsArray) {
      addStation(
        stationObj["name"].as<String>(),
        stationObj["url"].as<String>(),
        stationObj["icon"].as<String>(),
        stationObj["parent"].as<String>()
      );
    }
  }
  
  Serial.println("[STATION] ✓ Import complete");
  return true;
}

bool StationManagerClass::importStationsCSV(const String& csvData) {
  // Parse CSV and import stations
  // Format: type,name,url,icon,parent_folder
  
  int lineStart = 0;
  int lineEnd = csvData.indexOf('\n');
  bool firstLine = true;
  
  while (lineEnd >= 0) {
    String line = csvData.substring(lineStart, lineEnd);
    line.trim();
    
    // Skip header line
    if (firstLine) {
      firstLine = false;
      lineStart = lineEnd + 1;
      lineEnd = csvData.indexOf('\n', lineStart);
      continue;
    }
    
    // Parse CSV line
    int col = 0;
    int colStart = 0;
    String type, name, url, icon, parent;
    
    for (int i = 0; i <= line.length(); i++) {
      if (i == line.length() || line[i] == ',') {
        String value = line.substring(colStart, i);
        value.trim();
        
        switch (col) {
          case 0: type = value; break;
          case 1: name = value; break;
          case 2: url = value; break;
          case 3: icon = value; break;
          case 4: parent = value; break;
        }
        
        colStart = i + 1;
        col++;
      }
    }
    
    // Add station or folder
    if (type == "folder") {
      addFolder(name, icon, parent.length() > 0 ? parent : "/");
    } else if (type == "station") {
      addStation(name, url, icon, parent.length() > 0 ? parent : "/");
    }
    
    lineStart = lineEnd + 1;
    lineEnd = csvData.indexOf('\n', lineStart);
  }
  
  Serial.println("[STATION] ✓ CSV import complete");
  return true;
}

String StationManagerClass::exportStations() {
  // Export all stations as JSON
  DynamicJsonDocument doc(8192);
  
  // Export folders
  JsonArray foldersArray = doc.createNestedArray("folders");
  for (const auto& folder : folders) {
    JsonObject folderObj = foldersArray.createNestedObject();
    folderObj["name"] = folder.name;
    folderObj["iconPath"] = folder.iconPath;
    folderObj["parent"] = folder.parentFolder;
  }
  
  // Export stations
  JsonArray stationsArray = doc.createNestedArray("stations");
  for (const auto& station : stations) {
    JsonObject stationObj = stationsArray.createNestedObject();
    stationObj["name"] = station.name;
    stationObj["url"] = station.url;
    stationObj["icon"] = station.iconPath;
    stationObj["parent"] = station.parentFolder;
  }
  
  String jsonString;
  serializeJsonPretty(doc, jsonString);
  
  return jsonString;
}

void StationManagerClass::clearAll() {
  stations.clear();
  folders.clear();
  navigationStack.clear();
  currentFolder = "/";
  nextStationId = 1;
  nextFolderId = 1;
  
  saveStations();
  
  Serial.println("[STATION] All stations and folders cleared");
}

// ============================================================================
// Private Helper Functions
// ============================================================================

String StationManagerClass::getFullPath(const String& parentFolder, const String& name) {
  if (parentFolder == "/") {
    return "/" + name;
  } else {
    return parentFolder + "/" + name;
  }
}

std::vector<Station*> StationManagerClass::getStationsInFolder(const String& folderPath) {
  std::vector<Station*> result;
  
  for (auto& station : stations) {
    if (station.parentFolder == folderPath) {
      result.push_back(&station);
    }
  }
  
  return result;
}

std::vector<Folder*> StationManagerClass::getFoldersInFolder(const String& folderPath) {
  std::vector<Folder*> result;
  
  for (auto& folder : folders) {
    if (folder.parentFolder == folderPath) {
      result.push_back(&folder);
    }
  }
  
  return result;
}

bool StationManagerClass::isValidPath(const String& path) {
  // Simple path validation
  return path.startsWith("/");
}

void StationManagerClass::updateFolderCounts(const String& folderPath) {
  for (auto& folder : folders) {
    String fullPath = getFullPath(folder.parentFolder, folder.name);
    if (fullPath == folderPath) {
      folder.stationCount = getStationsInFolder(fullPath).size();
      folder.folderCount = getFoldersInFolder(fullPath).size();
    }
  }
}
