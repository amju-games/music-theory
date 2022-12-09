// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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
};

}
