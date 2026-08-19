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

  HudNumber& GetPlayerScore();
  HudNumber& GetPlayerLife(); // TODO deprecate: don't access internals 

  // Multiply the current points multiplier by the given value, and
  //  reset the countdown timer.
  void MultPointsMultiplier(int mult);

  // Add points to player points score.
  // Go through this route rather than accessing the Player Score so the
  //  points multiplier can work.
  // Frames: number of of frames over which the addition counts up.
  // TODO Should be time, not frames.
  void AddToPlayerPoints(int add, int frames);
 
private: 
  // Set size of patch behind player numeric score, and any others in future.
  void SetPatchSizes();

private:
  HudImpl* m_pimpl;
};

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud();
}

