// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorModeLerpCam.h"

namespace Amju
{
// * CorridorModeExitTappable *
// Transition back from focusing on a tappable, to re-entering Wait mode
class CorridorModeExitTappable : public CorridorModeLerpCam
{
public:
  static const int ID;
  void OnActive() override;
  void OnFinishedLerp() override;

protected:
  void SetCamLerpT() override;
};

}
