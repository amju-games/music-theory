// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <map>
#include <RCPtr.h>

namespace Amju
{
class ConfigFile;

// * UserProfile *
// This represents the student's progress through a Course.
// And other non-course info, e.g. avatar, student name, etc? This could perhaps
//  live in the GameConfigFile..?
class UserProfile : public RefCounted
{
public:
  bool Save();

  ConfigFile* GetConfigForTopic(const std::string& topicId);

private:
  std::map<std::string, RCPtr<ConfigFile>> m_configFiles;
};

// Get profile for current user, TODO might redo this a bit
UserProfile* TheUserProfile();
}
