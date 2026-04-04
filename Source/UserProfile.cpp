// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <ConfigFile.h>
#include <Directory.h>
#include <ReportError.h>
#include "Consts.h"
#include "UserProfile.h"

namespace Amju
{

namespace
{
const char* FILENAME_SUFFIX = "_user_profile.txt";
} // anon namespace

UserProfile* TheUserProfile()
{
  // TODO Different named profiles
  static UserProfile up;
  return &up;
}

bool UserProfile::Save()
{
  std::string filename = GetSaveDir(APPNAME) + FILENAME_SUFFIX;

std::cout << "Saving config file " << filename << "\n";

  if (!GetConfigFile()->Save(filename, false))
  {
    return false;
  }

  return true;
}

ConfigFile* UserProfile::GetConfigFile()
{
  // TODO The user profile isn't per-user, sigh
  static ConfigFile* cf = nullptr;
  if (!cf)
  { 
    cf = new ConfigFile;
    // If load fails, we assume first time getting config for this topic
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
  cf->SetInt(completed(m_name), m_completed ? 0 : 1);
  cf->SetInt(like(m_name), m_like ? 0 : 1);
}

SongPlayerInfo UserProfile::GetSongPlayerInfo(const std::string songName)
{
  SongPlayerInfo spi;
  spi.GetFromConfig(GetConfigFile());
  return spi;
}

void UserProfile::SetSongPlayerInfo(const SongPlayerInfo& spi)
{
  spi.SetToConfig(GetConfigFile());
}
}

