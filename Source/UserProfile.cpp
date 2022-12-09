// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <Directory.h>
#include "UserProfile.h"

namespace Amju
{
static const char* APP_NAME = "amjula-music-theory";

UserProfile* TheUserProfile()
{
  static UserProfile up;
  return &up;
}

bool UserProfile::Save()
{
  // Save all config files
  for (auto p : m_configFiles)
  {
    // TODO user name?
    std::string filename = GetSaveDir(APP_NAME) + p.first + ".txt";
    if (!p.second->Save(filename, false))
    {
      return false;
    }
  }
  return false;
}

ConfigFile* UserProfile::GetConfigForTopic(const std::string& topicId)
{
  // If it's in the map, return it
  auto it = m_configFiles.find(topicId);
  if (it != m_configFiles.end())
  {
    return it->second;
  }

  // Not in map, try to load, add to map
  ConfigFile* cf = new ConfigFile;
  m_configFiles[topicId] = cf;
  // If load fails, we assume first time getting config for this topic
  std::string filename = GetSaveDir(APP_NAME) + topicId + ".txt";

  if (!cf->Load(filename, false))
  {
    std::cout << "Failed to load topic config file " << filename << "\n";
  }

  return cf;
}

}
