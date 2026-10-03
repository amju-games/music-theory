// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include "Hud.h"

namespace Amju
{
// Mixin class for Game States with HUD.
// GUI for subclasses should include the HUD gui.
class WithHud 
{
public:
  virtual ~WithHud() = default;

  void UpdateHud();

  // Call after OnActive()
  void InitHud(PGuiElement gui, bool reset = false);
};
}

