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
// The user's progress through the topic is not represented here either. In
//  other words, the Topic is 'const' data, not the dynamic state.
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

  const std::vector<std::string>& GetTopicsThisUnlocks() const;

  // Called when user successfully completes this topic - which we expect will 
  //  unlock other topics
  void OnCompleted();

  int GetNumPages() const;
  Page* GetPage(int n);

protected:
  std::string m_id; // Unique ID

  // Localised name user sees
  std::string m_displayName;

  // IDs of topics which are unlocked when this topic is completed
  std::vector<std::string> m_unlocks;

  // The pages which present this topic, one page at a time.
  std::vector<RCPtr<Page>> m_pages;
};
}
