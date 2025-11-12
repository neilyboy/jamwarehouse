/**
 * Audio Player for Jam Wysteria
 * 
 * Handles internet radio streaming using ESP32-audioI2S library
 * with I2S output to MAX98357A amplifier
 */

#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "Audio.h"
#include "config.h"

class AudioPlayerClass {
public:
  AudioPlayerClass();
  
  // Initialization
  void init();
  
  // Playback control
  bool playStation(const String& url);
  void pause();
  void resume();
  void stop();
  bool isPlaying();
  bool isPaused();
  
  // Track control (for playlist streams)
  void next();
  void previous();
  bool canSkip();
  
  // Volume control
  void setVolume(int volume);
  int getVolume();
  void volumeUp();
  void volumeDown();
  void mute();
  void unmute();
  bool isMuted();
  
  // Stream info
  StreamMetadata getMetadata();
  String getTitle();
  String getArtist();
  String getAlbum();
  int getBitrate();
  bool hasMetadata();
  
  // Update (call in loop)
  void update();
  
  // Connection status
  bool isConnected();
  String getError();
  
private:
  Audio audio;
  
  // Playback state
  bool playing;
  bool paused;
  bool muted;
  int currentVolume;
  int volumeBeforeMute;
  
  // Current stream info
  String currentURL;
  StreamMetadata metadata;
  bool metadataAvailable;
  
  // Error tracking
  String lastError;
  bool hasError;
  
  // Helper functions
  void clearMetadata();
  void updateMetadata();
};

// Global instance
extern AudioPlayerClass AudioPlayer;

// Audio callbacks (must be global functions)
void audio_info(const char *info);
void audio_id3data(const char *info);
void audio_eof_mp3(const char *info);
void audio_showstation(const char *info);
void audio_showstreamtitle(const char *info);
void audio_bitrate(const char *info);
void audio_commercial(const char *info);
void audio_icyurl(const char *info);
void audio_lasthost(const char *info);

#endif // AUDIO_PLAYER_H
