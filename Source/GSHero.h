// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecAnimation.h>
#include <Singleton.h>
#include "GSBase.h"
#include "GuiMusicKb.h"
#include "GuiScrollScore.h"

namespace Amju
{
struct Grade;

// * GSHero *
// Hero mode: the game mechanic which is like Guitar Hero, but for keyboard/sheet music.
class GSHero : public GSBase
{
public:
  GSHero();

  void OnActive() override;
  void Update() override;
  void OnMusicKbEvent(const MusicKbEvent&) override;

  // Start the score scrolling, and backing track playing
  void Start();

  // Callback, when count-in finishes
  void OnCountInFinished();

protected:
  void InitGui();
 
  // Grade player input event against what we think is the corresponding
  //  event in the score.
  void GradeEvent(const MusicKbEvent& e);

  // Display feedback
  void FeedbackBalloon(const Grade& g);

  void ShowFeedbackBalloon(bool showNotHide);

  // Debug: called when R key is pressed to reload everything.
  void ReloadGui() override;

protected:
  // The scroll score child of m_gui (get after gui is loaded)
  RCPtr<GuiScrollScore> m_scrollScore;

  // The animator parent of scroll score
  RCPtr<GuiDecAnimation> m_scoreAnim;

  // The keyboard
  RCPtr<GuiMusicKb> m_keyboard;
};

typedef Singleton<GSHero> TheGSHero;
}
