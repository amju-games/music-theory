// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiMusicScore;

// * GSShowMusicScore *
// Show a musical score on screen.
class GSShowMusicScore : public GSBase
{
public:
  GSShowMusicScore();
  virtual void Update() override;
  virtual void OnActive() override;

private:
  void SetOctaveDot();

private:
  // Score scrolls to the left
  // The score is within the m_gui tree, so this is a non-ownership pointer.
  GuiMusicScore* m_score = nullptr;

  // BAD: wrong place for this
  // 7 gui images used to show the current octave
  GuiElement* m_dots[7];
};

typedef Singleton<GSShowMusicScore> TheGSShowMusicScore;
}
