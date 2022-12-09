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

  // Set camera desired pos and target 
  void SetDesired(const Vec3f& eye, const Vec3f& target);

  // Set camera desired pos and target from a tappable 
  void SetDesiredFromTappable(RCPtr<Tappable> tappable);

  // Set lerp value, 0..1: 0 means corridor view, 1 means tappable view
  void SetLerpT(float t);

  // Call to stop lerping camera
  void SetNoLerp();

private:
  // Store camera info for when we interpolate to tappable camera
  Vec3f m_origCamEye;
  Vec3f m_origCamTarget;

  Vec3f m_desiredCamEye;
  Vec3f m_desiredCamTarget;

  bool m_isCamLerping = false;
  float m_camLerpT = 0;

  // Shouldn't need this
  // TODO
//  RCPtr<Tappable> m_tappable;
};
}
