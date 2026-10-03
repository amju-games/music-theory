// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <Singleton.h>
#include "GSBase.h"
#include "WithHud.h"

namespace Amju
{
// * GSPause *
// Pause menu state, reached when learning a topic. 
// Options to go back to main menu, etc.
class GSPause : public GSBase, public WithHud
{
public:
  GSPause();

  void Update() override;
  void OnActive() override;
};

typedef Singleton<GSPause> TheGSPause;
}

