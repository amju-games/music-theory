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

  // Each Topic has a config file storing player-specific progress
  ConfigFile* GetConfigForTopic(const std::string& topicId);

  // Get/set current topic this play session.
  // Not persistent.
  void SetCurrentTopic(int topicId);
  int GetCurrentTopic() const;

  // Convenience function - move it
  std::string GetCurrentTopicDisplayName();

  // Get persistent total score over player lifetime
  // (On local device, right?)
  int GetScore(); // not const, may load config file
  void AddToScore(int addition);

  // Set score for the current topic.
  // Not persistent, is added once to persistent score.
  void SetTopicScore(int topicScore);
  int GetTopicScore() const;
  void AddTopicScoreToPersistentScore();

  // Get/Inc/Dec persistent hint count
  int GetHints(); // not const, may load config file
  // Add to persistent hint count: can be negative.
  void AddHints(int addition);

private:
  std::map<std::string, RCPtr<ConfigFile>> m_configFiles;

  int m_currentTopic = 0; // not persistent
  int m_topicScore = 0; // score for the current topic, not persistent
};

// Get profile for current user, TODO might redo this a bit
UserProfile* TheUserProfile();
}
