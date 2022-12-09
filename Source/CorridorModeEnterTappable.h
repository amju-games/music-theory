// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Vec3.h>
#include "CorridorMode.h"

namespace Amju
{
// * CorridorModeEnterTappable *
// Transition to showing tappable
class CorridorModeEnterTappable : public CorridorMode
{
public:
  static const int ID = 4;
  virtual void OnActive() override;
  virtual void Update() override;

private:
  // Store camera info for when we interpolate to tappable camera
  Vec3f m_origCamEye;
  Vec3f m_origCamTarget;
  bool m_isCamLerping = false;
  float m_camLerpTime = 0;
};

}
