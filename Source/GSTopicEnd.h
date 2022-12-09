// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Singleton.h>
#include "GSBase3d.h"

namespace Amju
{
// * GSTopicEnd *
// Shown when we finish a topic.
// Show score, rating, give hints, etc.
class GSTopicEnd : public GSBase3d
{
public:
  GSTopicEnd();
  virtual void OnActive() override;

  // Called on timer
  void UpdateNums();

private:
  std::string GenerateScoreComment();
  void SetScoreNumbers();
  void SetHintNumbers();

private:
  // Temp copies of player score etc, used to update GuiTexts
  int m_topicScore = 0;
  int m_totalScore = 0;
  int m_hints = 0;
  int m_finalHints = 0;
};

typedef Singleton<GSTopicEnd> TheGSTopicEnd;
}

