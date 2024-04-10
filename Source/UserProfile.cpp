// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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
const int MAX_NUM_HINTS = 10; // TODO OK?

const char* HINTS_AVAILABLE_KEY = "hints_avail";

// Start life with 3 hints
int DEFAULT_HINTS_AVAIL = 3;

const char* PLAYER_SCORE_KEY = "score";

const char* KEY_TOPIC_BEST = "topic_best_";

const char* KEY_TOPIC_UNLOCKED = "topic_unlocked_";

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

void UserProfile::SetTopicScore(int score, const std::string& topicId)
{
  Assert(!topicId.empty());
  m_topicScore = score;

  auto userConfig = GetConfigFile();
  int best = userConfig->GetInt(KEY_TOPIC_BEST + topicId, 0);
  if (m_topicScore > best)
  {

std::cout << "New best score for " << m_currentTopic << ": " 
  << m_topicScore << "\n";

    userConfig->SetInt(KEY_TOPIC_BEST + topicId, m_topicScore);
    Save();
  } 
}

int UserProfile::GetCurrentTopicScore() const
{
  return m_topicScore;
}

void UserProfile::SetCurrentTopic(int topicIndex)
{ 
  m_currentTopic = topicIndex;
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

int UserProfile::GetHints(HintType ht)
{
  auto userConfig = GetConfigFile();
  const std::string key = HINTS_AVAILABLE_KEY + ToString(static_cast<int>(ht));
  return userConfig->GetInt(key, DEFAULT_HINTS_AVAIL);
}

void UserProfile::AddHints(HintType ht, int add)
{
  auto userConfig = GetConfigFile();
  const std::string key = HINTS_AVAILABLE_KEY + ToString(static_cast<int>(ht));
  int hints = userConfig->GetInt(key, DEFAULT_HINTS_AVAIL);
  const int prevHints = hints;

  hints += add;
  if (hints < 0)
  {
    Assert(0); // -ve hints not allowed
    hints = 0;
  }

  if (hints > MAX_NUM_HINTS)
  {
    hints = MAX_NUM_HINTS;
  }

  if (hints != prevHints)
  {
    userConfig->SetInt(key, hints);
    Save();
  }
}

int UserProfile::GetBestTopicScore(const std::string& topicId) 
{
  Assert(!topicId.empty());
  auto userConfig = GetConfigFile();
  int best = userConfig->GetInt(KEY_TOPIC_BEST + topicId, 0);
  return best;
}

bool UserProfile::IsTopicUnlocked(const std::string& topicId) 
{
  Assert(!topicId.empty());
  auto userConfig = GetConfigFile();
  return userConfig->Exists(KEY_TOPIC_UNLOCKED + topicId);
}

bool UserProfile::WasTopicEverPassed(const std::string& topicId) 
{
  Assert(!topicId.empty());
  int best = GetBestTopicScore(topicId);
  const int TOPIC_PASS_MARK = 70; // TODO ok here? Per-topic??
  return best > TOPIC_PASS_MARK;
}

bool UserProfile::IsCurrentTopicPassed() const 
{
  // We want to know if the current attempt is a pass, not 
  //  whether the topic was _ever_ passed
  const int TOPIC_PASS_MARK = 70; // TODO ok here? Per-topic??
  return m_topicScore > TOPIC_PASS_MARK;
}

void UserProfile::UnlockTopic(const std::string& topicId)
{
  Assert(!topicId.empty());
  auto userConfig = GetConfigFile();
  userConfig->SetInt(KEY_TOPIC_UNLOCKED + topicId, 1);
}
}

