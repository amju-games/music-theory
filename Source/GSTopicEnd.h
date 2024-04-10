// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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
  void Draw() override; // camera shake
  void OnActive() override;

  // Called on timer
  void UpdateNums();
 
  void SetBestScore(int best);

private:
  std::string GenerateScoreComment();
  void SetScoreNumbers();
  void SetHintNumbers();

private:
  // Temp copies of player score etc, used to update GuiTexts
  int m_topicScore = 0;
  int m_totalScore = 0;
  int m_hints = 0;

  // So we can show best ever score before and after
  int m_bestScore = 0;

  // Play ascending midi notes as we increment score display
  int m_midiNote = 0;

  // TODO Currently not used
//  float m_shakeTime = 0;
//  bool m_isShakingCamera = false;
};

typedef Singleton<GSTopicEnd> TheGSTopicEnd;
}

