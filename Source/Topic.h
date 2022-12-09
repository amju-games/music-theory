// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <string>
#include <vector>
#include <RCPtr.h>
#include "Page.h"

namespace Amju
{
class Course;
class File;

// * Topic *
// Represents a unit of teaching/learning, (which for this app we expect to be 
//  short, taking a few minutes to complete).
// The topic contains details of what is to be learned, but not its presentation - 
//  this is handled by Pages.
// The user's progress through the topic is not represented here either. This is
//  'const' data, not the dynamic state.
class Topic : public RefCounted
{
public:
  Topic() = default;
  Topic(const std::string& id);

  virtual ~Topic() = default;

  virtual bool Load(File*);

  // Unique ID for the Topic, so we can access specific topics we want to unlock.
  // Make this a string so we can understand what the IDs mean in text files.
  const std::string& GetId() const;
  void SetId(const std::string& id);

  // Localised string to show to user
  const std::string& GetDisplayName() const;
  void SetDisplayName(const std::string&);

  void AddTopicThisUnlocks(const std::string& unlocks);

  // Called when user successfully completes this topic - which we expect will 
  //  unlock other topics
  void OnCompleted();

  int GetNumPages() const;
  Page* GetPage(int n);

  const std::string& GetGroup() const;

protected:
  // The Course which owns this topic.
  // Weak ptr - Course has ref counted ptr to this topic.
  Course* m_userProfile = nullptr;

  std::string m_id; // Unique ID

  // Localised name user sees
  std::string m_displayName;

  // Group to which this topic belongs, so we can present Topics in a
  //  structured way. But as far as the Topic and Course is concerned, this 
  //  is just an arbitrary string.
  std::string m_group;

  // IDs of topics which are unlocked when this topic is completed
  std::vector<std::string> m_unlocks;

  // The pages which present this topic, one page at a time.
  std::vector<RCPtr<Page>> m_pages;
};
}
