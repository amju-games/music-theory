// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <ConfigFile.h>
#include <Directory.h>
#include <ReportError.h>
#include "Consts.h"
#include "HeroGameRound.h"
#include "UserProfile.h"

namespace Amju
{

namespace
{
const char* FILENAME_SUFFIX = "_user_profile.txt";
} // anon namespace

// Default player name, meaning 'for all players'
static std::string s_playerName = "all";

void SetPlayerName(const std::string& playerName)
{
  s_playerName = playerName;
}

UserProfile* GetUserProfile()
{
  // TODO Different named profiles
  static UserProfile up;
  return &up;
}

bool UserProfile::Save()
{
  std::string filename = GetSaveDir(APPNAME) + s_playerName + FILENAME_SUFFIX;

std::cout << "Saving config file " << filename << "\n";

  if (!GetConfigFile()->Save(filename, false))
  {
    return false;
  }

  return true;
}

ConfigFile* MockUserProfile::GetConfigFile()
{
  static ConfigFile cf;
  return &cf;
}

ConfigFile* UserProfile::GetConfigFile()
{
  // TODO The user profile isn't per-user, sigh
  static ConfigFile* cf = nullptr;
  if (!cf)
  { 
    cf = new ConfigFile;
    // If load fails, we assume first time getting config for this player
    std::string filename = GetSaveDir(APPNAME) + FILENAME_SUFFIX;

    if (!cf->Load(filename, false))
    {
      ReportError("Failed to load config file " + filename);
    }
  }

  return cf;
}

std::string hiScore(const std::string prefix) { return prefix + "-hiscore"; }
std::string completed(const std::string prefix) { return prefix + "-completed"; }
std::string like(const std::string prefix) { return prefix + "-like"; }

void SongPlayerInfo::GetFromConfig(const ConfigFile* cf)
{
  m_hiScore = cf->GetInt(hiScore(m_name), 0);
  m_completed = cf->GetInt(completed(m_name), 0) != 0;
  m_like = cf->GetInt(like(m_name), 0) != 0;
}

void SongPlayerInfo::SetToConfig(ConfigFile* cf) const
{
  cf->SetInt(hiScore(m_name), m_hiScore);
  cf->SetInt(completed(m_name), m_completed ? 1 : 0);
  cf->SetInt(like(m_name), m_like ? 1 : 0);
}

SongPlayerInfo UserProfile::GetSongPlayerInfo(const std::string songName)
{
  SongPlayerInfo spi;
  spi.m_name = songName;
  spi.GetFromConfig(GetConfigFile());
  return spi;
}

void UserProfile::SetSongPlayerInfo(const SongPlayerInfo& spi)
{
  spi.SetToConfig(GetConfigFile());
}

// From the songs manager and user profile, return the names of
//  unlocked songs.
std::set<std::string> CalcUnlockedSongNames(
  GameRoundManager* grm, UserProfile* user)
{
  std::set<std::string> result;

  // Max unlocked level: is incremented if the current level
  //  has all its songs completed.
  int maxUnlockedLevel = -1; 
  int level = -1; // current level as we loop through all songs

  // Flag keeping track of completed songs in the current level.
  // Set to false as soon as we find an uncompleted song.
  bool allSongsInThisLevelAreCompleted = true;

  int numSongs = grm->GetNumGameRounds();

  for (int i = 0; i < numSongs; i++)
  {
    const auto& gameround = grm->GetGameRound(i);
    auto songPlayerInfo = user->GetSongPlayerInfo(gameround.m_name);

    if (gameround.m_level != level)
    {   
      if (allSongsInThisLevelAreCompleted) // well, previous level now.
      {   
        ++maxUnlockedLevel;
      }   

      level = gameround.m_level;
      allSongsInThisLevelAreCompleted = true; // reset flag for new level
    }   


    allSongsInThisLevelAreCompleted &= songPlayerInfo.m_completed;

    // Add song name to result if unlocked
    bool isUnlocked = (level <= maxUnlockedLevel);
    if (isUnlocked)
    {   
      result.insert(gameround.m_name);
    }   
  }
  return result;
}
}

