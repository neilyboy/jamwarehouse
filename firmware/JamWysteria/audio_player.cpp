/**
 * Audio Player Implementation
 */

#include "audio_player.h"

// Global instance
AudioPlayerClass AudioPlayer;

AudioPlayerClass::AudioPlayerClass() :
  audio(),
  playing(false),
  paused(false),
  muted(false),
  currentVolume(VOLUME_DEFAULT),
  volumeBeforeMute(VOLUME_DEFAULT),
  currentURL(""),
  metadataAvailable(false),
  lastError(""),
  hasError(false) {
}

void AudioPlayerClass::init() {
  // Initialize I2S audio output
  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
  
  // Set initial volume
  audio.setVolume(currentVolume);
  
  // Set buffer size
  audio.setConnectionTimeout(STREAM_CONNECT_TIMEOUT, STREAM_RECONNECT_DELAY);
  
  Serial.println("[AUDIO] Initialized");
  Serial.printf("[AUDIO] I2S Pins - BCLK:%d, LRC:%d, DOUT:%d\n", 
                I2S_BCLK, I2S_LRC, I2S_DOUT);
  Serial.printf("[AUDIO] Default volume: %d/%d\n", currentVolume, VOLUME_MAX);
}

bool AudioPlayerClass::playStation(const String& url) {
  Serial.printf("[AUDIO] Playing: %s\n", url.c_str());
  
  // Stop current playback
  if (playing) {
    stop();
  }
  
  // Clear metadata
  clearMetadata();
  
  // Clear error
  hasError = false;
  lastError = "";
  
  // Start new stream
  if (audio.connecttohost(url.c_str())) {
    currentURL = url;
    playing = true;
    paused = false;
    
    Serial.println("[AUDIO] ✓ Stream connected");
    return true;
  } else {
    hasError = true;
    lastError = "Failed to connect to stream";
    Serial.println("[AUDIO] ✗ Connection failed");
    return false;
  }
}

void AudioPlayerClass::pause() {
  if (playing && !paused) {
    audio.pauseResume();
    paused = true;
    Serial.println("[AUDIO] Paused");
  }
}

void AudioPlayerClass::resume() {
  if (playing && paused) {
    audio.pauseResume();
    paused = false;
    Serial.println("[AUDIO] Resumed");
  }
}

void AudioPlayerClass::stop() {
  if (playing) {
    audio.stopSong();
    playing = false;
    paused = false;
    currentURL = "";
    clearMetadata();
    
    Serial.println("[AUDIO] Stopped");
  }
}

bool AudioPlayerClass::isPlaying() {
  return playing && !paused;
}

bool AudioPlayerClass::isPaused() {
  return paused;
}

void AudioPlayerClass::next() {
  // Next track (if supported by stream/playlist)
  // For most live streams, this won't do anything
  Serial.println("[AUDIO] Next track requested");
  // audio.next(); // If supported by library
}

void AudioPlayerClass::previous() {
  // Previous track (if supported by stream/playlist)
  Serial.println("[AUDIO] Previous track requested");
  // audio.previous(); // If supported by library
}

bool AudioPlayerClass::canSkip() {
  // Most live streams don't support skipping
  return false;
}

void AudioPlayerClass::setVolume(int volume) {
  // Clamp volume
  if (volume < VOLUME_MIN) volume = VOLUME_MIN;
  if (volume > VOLUME_MAX) volume = VOLUME_MAX;
  
  currentVolume = volume;
  
  if (!muted) {
    audio.setVolume(volume);
  }
  
  #ifdef DEBUG_MODE
  Serial.printf("[AUDIO] Volume: %d/%d\n", volume, VOLUME_MAX);
  #endif
}

int AudioPlayerClass::getVolume() {
  return currentVolume;
}

void AudioPlayerClass::volumeUp() {
  setVolume(currentVolume + VOLUME_STEP);
}

void AudioPlayerClass::volumeDown() {
  setVolume(currentVolume - VOLUME_STEP);
}

void AudioPlayerClass::mute() {
  if (!muted) {
    volumeBeforeMute = currentVolume;
    audio.setVolume(0);
    muted = true;
    Serial.println("[AUDIO] Muted");
  }
}

void AudioPlayerClass::unmute() {
  if (muted) {
    audio.setVolume(volumeBeforeMute);
    muted = false;
    Serial.println("[AUDIO] Unmuted");
  }
}

bool AudioPlayerClass::isMuted() {
  return muted;
}

StreamMetadata AudioPlayerClass::getMetadata() {
  return metadata;
}

String AudioPlayerClass::getTitle() {
  return metadata.title;
}

String AudioPlayerClass::getArtist() {
  return metadata.artist;
}

String AudioPlayerClass::getAlbum() {
  return metadata.album;
}

int AudioPlayerClass::getBitrate() {
  return metadata.bitrate;
}

bool AudioPlayerClass::hasMetadata() {
  return metadataAvailable;
}

void AudioPlayerClass::update() {
  // Update audio loop (must be called regularly)
  if (playing) {
    audio.loop();
  }
}

bool AudioPlayerClass::isConnected() {
  return playing && !hasError;
}

String AudioPlayerClass::getError() {
  return lastError;
}

void AudioPlayerClass::clearMetadata() {
  metadata.title = "";
  metadata.artist = "";
  metadata.album = "";
  metadata.genre = "";
  metadata.bitrate = 0;
  metadata.hasAlbumArt = false;
  metadata.albumArtURL = "";
  metadataAvailable = false;
}

void AudioPlayerClass::updateMetadata() {
  metadataAvailable = true;
}

// ============================================================================
// Audio Callback Functions (Global)
// ============================================================================

void audio_info(const char *info) {
  #ifdef DEBUG_MODE
  Serial.print("[AUDIO INFO] ");
  Serial.println(info);
  #endif
}

void audio_id3data(const char *info) {
  Serial.print("[AUDIO ID3] ");
  Serial.println(info);
  
  // Parse ID3 data and update metadata
  // TODO: Parse ID3 tags
}

void audio_eof_mp3(const char *info) {
  Serial.println("[AUDIO] End of file");
}

void audio_showstation(const char *info) {
  Serial.print("[AUDIO STATION] ");
  Serial.println(info);
  
  AudioPlayer.getMetadata().title = String(info);
}

void audio_showstreamtitle(const char *info) {
  Serial.print("[AUDIO STREAM] ");
  Serial.println(info);
  
  // Parse stream title (usually "Artist - Title")
  String streamTitle = String(info);
  int separatorPos = streamTitle.indexOf(" - ");
  
  if (separatorPos > 0) {
    AudioPlayer.getMetadata().artist = streamTitle.substring(0, separatorPos);
    AudioPlayer.getMetadata().title = streamTitle.substring(separatorPos + 3);
  } else {
    AudioPlayer.getMetadata().title = streamTitle;
  }
}

void audio_bitrate(const char *info) {
  #ifdef DEBUG_MODE
  Serial.print("[AUDIO BITRATE] ");
  Serial.println(info);
  #endif
  
  AudioPlayer.getMetadata().bitrate = String(info).toInt();
}

void audio_commercial(const char *info) {
  Serial.println("[AUDIO] Commercial break");
}

void audio_icyurl(const char *info) {
  #ifdef DEBUG_MODE
  Serial.print("[AUDIO ICY URL] ");
  Serial.println(info);
  #endif
}

void audio_lasthost(const char *info) {
  #ifdef DEBUG_MODE
  Serial.print("[AUDIO HOST] ");
  Serial.println(info);
  #endif
}
