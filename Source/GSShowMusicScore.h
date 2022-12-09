// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiMusicScore;

class GSShowMusicScore : public GSBase
{
public:
  virtual void Update() override;
  virtual void OnActive() override;

private:
  // Score scrolls to the left
  // The score is within the m_gui tree, so this is a non-ownership pointer.
  GuiMusicScore* m_score = nullptr;
};

typedef Singleton<GSShowMusicScore> TheGSShowMusicScore;
}
