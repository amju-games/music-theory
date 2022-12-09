// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeLerpCam * 
// Base class for Corridor Modes which lerp a camera.
class CorridorModeLerpCam : public CorridorMode
{
public:
  void Update() override;
  void OnActive() override;
  virtual void OnFinishedLerp() = 0;

protected:
  float m_camLerpT = 0;
  float m_speed = 1.0f; // i.e. lerp 1..0 takes 1s, by default
};
}

