// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <RCPtr.h>

namespace Amju
{
class Tappable;

// * CorridorCamController *
// Control camera in corridor - scroll position, lerping to tappable view,
//  maybe other modes?
class CorridorCamController
{
public:
  void Update();

  // Set tappable to which we lerp
  void SetTappable(RCPtr<Tappable> tappable);

  // Set lerp value, 0..1: 0 means corridor view, 1 means tappable view
  void SetLerpT(float t);

private:
  // Store camera info for when we interpolate to tappable camera
  Vec3f m_origCamEye;
  Vec3f m_origCamTarget;

  bool m_isCamLerping = false;
  float m_camLerpT = 0;
  RCPtr<Tappable> m_tappable;
};
}
