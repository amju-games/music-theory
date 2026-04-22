// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <set>
#include <string>
#include <ConfigFile.h>
#include <RCPtr.h>

namespace Amju
{
// Represents player info for one song.
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
// This represents the player's progress through the game, 
//  it's a set of convenience functions around a ConfigFile.
class UserProfile : public RefCounted
{
public:
  virtual ~UserProfile() = default;

  // Override to mock for testing.
  virtual bool Save();

  // Get the song player info for the given song name, which is its unique ID.
  SongPlayerInfo GetSongPlayerInfo(const std::string songName);

  // Set song player info, having updated some member(s).
  void SetSongPlayerInfo(const SongPlayerInfo& spi);

  // Direct access to single config file for storing any persistent data
  // Override to mock for testing.
  virtual ConfigFile* GetConfigFile();

private:
  RCPtr<ConfigFile> m_configFile;
};

// For testing: doesn't access filesystem.
class MockUserProfile : public UserProfile
{
public:
  bool Save() override { return true; }
  ConfigFile* GetConfigFile() override;
};

// Get profile for current user, TODO might redo this a bit
UserProfile* GetUserProfile();

// Set player name, which is prepended to the user config filename.
// So we can have different configs for individual players - not
//  expecting to need to do this, on iOS.
void SetPlayerName(const std::string& playerName);

class GameRoundManager;
// From the given songs (game rounds) manager and user profile, 
//  return the names of unlocked songs.
std::set<std::string> CalcUnlockedSongNames(
  GameRoundManager* grm, UserProfile* user);
}

