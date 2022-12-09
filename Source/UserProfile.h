// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <map>
#include <RCPtr.h>

namespace Amju
{
class ConfigFile;

//// * UserTopicState *
//// The user's progress on a topic
//class UserTopicState : public RefCounted
//{
//public:
//
//protected:
//  std::string m_topicKey; // the topic for which we represent progress
//  bool m_isCompleted;
//  bool m_isUnlocked;
//};

// * UserProfile *
// This represents the student's progress through a Course.
// And other non-course info, e.g. avatar, student name, etc? This could perhaps
//  live in the GameConfigFile..?
class UserProfile : public RefCounted
{
public:
//??  bool Load();
  bool Save();

  ConfigFile* GetConfigForTopic(const std::string& topicId);

private:
  std::map<std::string, RCPtr<ConfigFile>> m_configFiles;
};

// Get profile for current user, TODO might redo this a bit
UserProfile* TheUserProfile();
}
