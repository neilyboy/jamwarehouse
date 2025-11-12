/**
 * Station Manager for Jam Wysteria
 * 
 * Manages radio stations and folders in a hierarchical structure
 */

#ifndef STATION_MANAGER_H
#define STATION_MANAGER_H

#include <vector>
#include "config.h"

class StationManagerClass {
public:
  StationManagerClass();
  
  // Initialization
  void init();
  
  // Load/Save
  bool loadStations();
  bool saveStations();
  
  // Station operations
  int addStation(const String& name, const String& url, const String& iconPath, const String& parentFolder);
  bool removeStation(int id);
  bool updateStation(int id, const String& name, const String& url, const String& iconPath);
  Station* getStation(int id);
  Station* getStationByIndex(int index);
  int getStationCount();
  
  // Folder operations
  int addFolder(const String& name, const String& iconPath, const String& parentFolder);
  bool removeFolder(int id);
  bool updateFolder(int id, const String& name, const String& iconPath);
  Folder* getFolder(int id);
  Folder* getFolderByIndex(int index);
  int getFolderCount();
  
  // Navigation
  void enterFolder(int folderId);
  void enterFolder(const String& folderPath);
  void goBack();
  bool canGoBack();
  String getCurrentPath();
  String getCurrentFolderName();
  
  // Get current view items
  std::vector<Folder*> getCurrentFolders();
  std::vector<Station*> getCurrentStations();
  
  // Search
  std::vector<Station*> searchStations(const String& query);
  
  // Bulk operations
  bool importStations(const String& jsonData);
  bool importStationsCSV(const String& csvData);
  String exportStations();
  
  // Clear all
  void clearAll();
  
private:
  std::vector<Station> stations;
  std::vector<Folder> folders;
  std::vector<String> navigationStack;
  String currentFolder;
  
  int nextStationId;
  int nextFolderId;
  
  // Helper functions
  String getFullPath(const String& parentFolder, const String& name);
  std::vector<Station*> getStationsInFolder(const String& folderPath);
  std::vector<Folder*> getFoldersInFolder(const String& folderPath);
  bool isValidPath(const String& path);
  void updateFolderCounts(const String& folderPath);
};

// Global instance
extern StationManagerClass StationManager;

#endif // STATION_MANAGER_H
