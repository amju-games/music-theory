// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <vector>
#include <RCPtr.h>

namespace Amju
{
// * UserTopicState *
// The user's progress on a topic
class UserTopicState : public RefCounted
{
public:

protected:
  std::string m_topicKey; // the topic for which we represent progress
  bool m_isCompleted;
  bool m_isUnlocked;
};

// * UserProfile *
// This represents the student's progress through a Course, and other non-course
//  info, e.g. avatar, student name, etc.
class UserProfile : public RefCounted
{
public:
  bool Load();
  bool Save();

private:
  std::vector<RCPtr<UserTopicState>> m_topicStates;
};
}
