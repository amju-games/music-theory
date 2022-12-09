// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <map>
#include <ConfigFile.h>
#include <RCPtr.h>
#include "HintType.h"

namespace Amju
{
// * UserProfile *
// This represents the student's progress through a Course.
// And other non-course info, e.g. avatar, student name, etc? This could perhaps
//  live in the GameConfigFile..?
class UserProfile : public RefCounted
{
public:
  bool Save();

  // Direct access to single config file for storing any persistent data
  ConfigFile* GetConfigFile();

  // Get/set current topic this play session.
  // Not persistent.
  void SetCurrentTopic(int topicId);
  int GetCurrentTopic() const;

  // Get current level - store in config file.
  void SetCurrentLevel(int level);
  int GetCurrentLevel() const;

  // Convenience function - move it
  std::string GetCurrentTopicDisplayName() const;

  // Get persistent total score over player lifetime
  // (On local device, right?)
  int GetScore(); // not const, may load config file
  void AddToScore(int addition);

  // Set score for the current topic. Update persistent best score
  //  if it's better than current best.
  void SetTopicScore(int score, const std::string& topicId);
  int GetCurrentTopicScore() const;

  // Persistent config for each topic
  int GetBestTopicScore(const std::string& topicId);
  bool IsTopicUnlocked(const std::string& topicId);
  bool IsTopicPassed(const std::string& topicId);
  void UnlockTopic(const std::string& topicId);

  // Get/Inc/Dec persistent hint count
  int GetHints(HintType ht); // not const, may load config file
  // Add to persistent hint count: can be negative.
  void AddHints(HintType ht, int addition);

  // Get persistent 'Play' button count (plays music in question)
  int GetPlays();
  void AddPlays(int addition);

private:
  RCPtr<ConfigFile> m_configFile;

  int m_currentTopic = 0; // not persistent
  int m_topicScore = 0; // score for the current topic, not persistent
};

// Get profile for current user, TODO might redo this a bit
UserProfile* TheUserProfile();
}
