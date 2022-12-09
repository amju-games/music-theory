// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <Directory.h>
#include <ReportError.h>
#include "Consts.h"
#include "Course.h"
#include "UserProfile.h"

namespace Amju
{
static const char* HINTS_AVAILABLE_KEY = "hints_avail";

// Start life with 3 hints
const int DEFAULT_HINTS_AVAIL = 3;

static const char* PLAYER_SCORE_KEY = "score";

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
    std::string filename = GetSaveDir(APPNAME) + p.first + ".txt";
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
  std::string filename = GetSaveDir(APPNAME) + topicId + ".txt";

  if (!cf->Load(filename, false))
  {
    ReportError("Failed to load topic config file " + filename);
  }

  return cf;
}

int UserProfile::GetScore() 
{
  auto userConfig = GetConfigForTopic(KEY_GENERAL);
  return userConfig->GetInt(PLAYER_SCORE_KEY, 0);
}

void UserProfile::AddToScore(int add)
{
  auto userConfig = GetConfigForTopic(KEY_GENERAL);
  int score = userConfig->GetInt(PLAYER_SCORE_KEY, 0);
  score += add;
  userConfig->SetInt(PLAYER_SCORE_KEY, score);
  Save();
}

void UserProfile::SetTopicScore(int topicScore)
{
  m_topicScore = topicScore;
}

int UserProfile::GetTopicScore() const
{
  return m_topicScore;
}

void UserProfile::AddTopicScoreToPersistentScore()
{
  AddToScore(m_topicScore);
  SetTopicScore(0);
}

void UserProfile::SetCurrentTopic(int topicId)
{ 
  m_currentTopic = topicId;
}

int UserProfile::GetCurrentTopic() const
{
  return m_currentTopic;
}

std::string UserProfile::GetCurrentTopicDisplayName()
{
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(m_currentTopic);
  Assert(topic);
  return topic->GetDisplayName();
}

int UserProfile::GetHints()
{
  auto userConfig = GetConfigForTopic(KEY_GENERAL);
  return userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
}

void UserProfile::AddHints(int add)
{
  auto userConfig = GetConfigForTopic(KEY_GENERAL);
  int hints = userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
  hints += add;
  if (hints < 0)
  {
    Assert(0); // -ve hints not allowed
    hints = 0;
  }
  userConfig->SetInt(HINTS_AVAILABLE_KEY, hints);
  Save();
}

}
