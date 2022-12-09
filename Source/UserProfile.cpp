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

namespace
{
const char* HINTS_AVAILABLE_KEY = "hints_avail";

// Start life with 3 hints
int DEFAULT_HINTS_AVAIL = 3;

const char* PLAYER_SCORE_KEY = "score";

const char* KEY_TOPIC_BEST = "topic_best_";

const char* FILENAME_SUFFIX = "_user_profile.txt";

} // anon namespace

UserProfile* TheUserProfile()
{
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
  // Not in map, try to load, add to map
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

int UserProfile::GetScore() 
{
  auto userConfig = GetConfigFile();
  return userConfig->GetInt(PLAYER_SCORE_KEY, 0);
}

//void UserProfile::AddToScore(int add)
//{
///  auto userConfig = GetConfigFile();
 // int score = userConfig->GetInt(PLAYER_SCORE_KEY, 0);
 // score += add;
 // userConfig->SetInt(PLAYER_SCORE_KEY, score);
 // Save();
//}

void UserProfile::SetTopicScore(int topicScore)
{
  m_topicScore = topicScore;

  auto userConfig = GetConfigFile();
  int best = userConfig->GetInt(KEY_TOPIC_BEST + m_currentTopic, 0);
  if (best > m_topicScore)
  {

std::cout << "New best score for " << m_currentTopic << ": " 
  << m_topicScore << "\n";

    userConfig->SetInt(KEY_TOPIC_BEST + m_currentTopic, m_topicScore);
    Save();
  } 
}

int UserProfile::GetCurrentTopicScore() const
{
  return m_topicScore;
}

void UserProfile::SetCurrentTopic(int topicId)
{ 
  m_currentTopic = topicId;
}

int UserProfile::GetCurrentTopic() const
{
  return m_currentTopic;
}

std::string UserProfile::GetCurrentTopicDisplayName() const
{
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(m_currentTopic);
  Assert(topic);
  return topic->GetDisplayName();
}

int UserProfile::GetHints()
{
  auto userConfig = GetConfigFile();
  return userConfig->GetInt(HINTS_AVAILABLE_KEY, DEFAULT_HINTS_AVAIL);
}

void UserProfile::AddHints(int add)
{
  auto userConfig = GetConfigFile();
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

int UserProfile::GetBestTopicScore(const std::string& topicId) 
{
  auto userConfig = GetConfigFile();
  int best = userConfig->GetInt(KEY_TOPIC_BEST + m_currentTopic, 0);
  return best;
}

bool UserProfile::IsTopicUnlocked(const std::string& topicId) 
{
  auto userConfig = GetConfigFile();
  return userConfig->Exists(KEY_TOPIC_UNLOCKED + topicId);
}

bool UserProfile::IsTopicPassed(const std::string& topicId) 
{
  int best = GetBestTopicScore(topicId);
  const int TOPIC_PASS_MARK = 1; // TODO ok here? Per-topic??
  return best > TOPIC_PASS_MARK;
}

void UserProfile::UnlockTopic(const std::string& topicId)
{
  auto userConfig = GetConfigFile();
  userConfig->SetInt(KEY_TOPIC_UNLOCKED + topicId, 1);
}
}

