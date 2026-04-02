#include "GSBaseWithHud.h"
#include "Hud.h"

namespace Amju
{
void GSBaseWithHud::Update() 
{
  GSBase::Update();
  GetHud().Update();
}

void GSBaseWithHud::InitHud(bool reset)
{
  GetHud().InitGui(m_gui, reset);
}
}
