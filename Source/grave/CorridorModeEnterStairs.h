// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "CorridorModeLerpCam.h"

namespace Amju
{
// * CorridorModeEnterStairs *
// Move camera into stairs, fade out or other effect
class CorridorModeEnterStairs : public CorridorModeLerpCam
{
public:
  static const int ID;
  virtual void OnActive() override;
  virtual void OnFinishedLerp() override;

  static void SetNewLevel(int newLevel);

private:
  static int s_newLevel;
};

}
