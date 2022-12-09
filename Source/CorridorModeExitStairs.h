// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorModeLerpCam.h"

namespace Amju
{
class CorridorModeExitStairs : public CorridorModeLerpCam
{
public:
  static const int ID;
  virtual void OnActive() override;
  virtual void OnFinishedLerp() override;

protected:
  void SetCamLerpT() override;
};

}
