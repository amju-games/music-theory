#pragma once

#include <GuiElement.h>

namespace Amju
{
// Pimpl idiom: reduce compile dependencies.
struct HudImpl;

// One integer value displayed on the HUD.
struct HudNumber;

// ** HUD **
// Head up display: score, life counter, etc.
struct Hud
{
public:
  Hud();
  ~Hud();

  void Update();
  // Call when gui is (re)loaded; optionally reset to new game values,
  //  or keep values from previous incarnation
  void InitGui(PGuiElement gui, bool reset); 

  // Set size of patch behind player numeric score, and any others in future.
  void SetPatchSizes();

  HudNumber& GetPlayerScore();
  HudNumber& GetPlayerLife();

private:
  HudImpl* m_pimpl;
};

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud();
}

