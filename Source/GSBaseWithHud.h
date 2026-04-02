#pragma once

#include "GSBase.h"
#include "Hud.h"

namespace Amju
{
// Base class for Game States with HUD.
// GUI for subclasses should include the HUD gui.
class GSBaseWithHud : public GSBase
{
public:
  void Update() override;

  // Call after OnActive()
  void InitHud(bool reset = false);
};
}

