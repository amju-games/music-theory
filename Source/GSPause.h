// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSPause *
// Pause menu state, reached when learning a topic. Options to go back to main menu, etc.
class GSPause : public GSBase
{
public:
  GSPause();
  virtual void OnActive() override;
};

typedef Singleton<GSPause> TheGSPause;
}

