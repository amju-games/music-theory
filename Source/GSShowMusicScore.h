// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GSShowMusicScore : public GSBase
{
public:
  virtual void Update() override;
  virtual void OnActive() override;

  // Reload
  virtual bool OnKeyEvent(const KeyEvent&) override;
};

typedef Singleton<GSShowMusicScore> TheGSShowMusicScore;
}
