// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <ConfigFile.h>
#include <RCPtr.h>

namespace Amju
{
struct SongPlayerInfo
{
  std::string m_name; // song name == unique ID
  int m_hiScore = 0;
  bool m_completed = false;
  bool m_like = false;
 
  void GetFromConfig(const ConfigFile*);
  void SetToConfig(ConfigFile*) const;
};

// * UserProfile *
// This represents the student's progress through the game.
class UserProfile : public RefCounted
{
public:
  bool Save();

  // Get the song player info for the given song name, which is its unique ID.
  SongPlayerInfo GetSongPlayerInfo(const std::string songName);

  // Set song player info, having updated some member(s).
  void SetSongPlayerInfo(const SongPlayerInfo& spi);

  // Direct access to single config file for storing any persistent data
  ConfigFile* GetConfigFile();

private:
  RCPtr<ConfigFile> m_configFile;
};

// Get profile for current user, TODO might redo this a bit
UserProfile* TheUserProfile();
}
