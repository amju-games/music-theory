#pragma once

#include "HudNumber.h"

namespace Amju
{
// ** HUD **
// Head up display: score, life counter, etc.
struct Hud
{
public:
  void Update();
  // Call when gui is (re)loaded; optionally reset to new game values,
  //  or keep values from previous incarnation
  void InitGui(PGuiElement gui, bool reset); 
 
  HudNumber m_playerScore;
  HudNumber m_playerLife;
};

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud();
}

