// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "CorridorModeLerpCam.h"

namespace Amju
{
// * CorridorModeEnterTappable *
// Transition to showing tappable
class CorridorModeEnterTappable : public CorridorModeLerpCam
{
public:
  static const int ID;
  virtual void OnActive() override;
  virtual void OnFinishedLerp() override;
};

}
