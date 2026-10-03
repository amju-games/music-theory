// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "WithHud.h"
#include "Hud.h"

namespace Amju
{
void WithHud::UpdateHud() 
{
  GetHud().Update();
}

void WithHud::InitHud(PGuiElement gui, bool reset)
{
  GetHud().InitGui(gui, reset);
}
}
