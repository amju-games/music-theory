// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecAnimation.h>
#include <Singleton.h>
#include "GSBase.h"
#include "GuiScrollScore.h"

namespace Amju
{
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

protected:
  // Get the scroll score child of m_gui after it's loaded
  RCPtr<GuiScrollScore> m_scrollScore;
  // Get animator parent of scroll score
  RCPtr<GuiDecAnimation> m_scoreAnim;
};

typedef Singleton<GSHero> TheGSHero;
}
