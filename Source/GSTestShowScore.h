// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiMusicScore;

// * GSTestShowScore *
// Test: show static display of music score, loaded from file.
class GSTestShowScore : public GSBase
{
public:
  GSTestShowScore();
  virtual void OnActive() override;
};

typedef Singleton<GSTestShowScore> TheGSTestShowScore;
}
