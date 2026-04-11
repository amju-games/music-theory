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
