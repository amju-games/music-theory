// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase3d.h"
#include <Singleton.h>

namespace Amju
{
// * GSTopicStart *
// Shown when we start learning a topic. The user might have already seen this
//  topic, or it might be new (TODO different states for these scenarios?)
// Display the localised topic name (and introduction?).
// Create/Load pages for the topic, go to Pages state.
class GSTopicStart : public GSBase3d
{
public:
  GSTopicStart();
  virtual void OnActive() override;

  void SetTopic(int topicNum);
  int GetTopic() const;

private:
  int m_topicNum = -1;
};

typedef Singleton<GSTopicStart> TheGSTopicStart;
}

