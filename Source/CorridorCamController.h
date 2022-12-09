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

  // Set lerp value, 0..1: 0 means original view, 1 means desired view
  void SetLerpT(float t);

  // Call to stop updating the camera
  void SetNoLerp();

private:
  // Store camera info for when we interpolate to tappable camera
  Vec3f m_origCamEye;
  Vec3f m_origCamTarget;

  Vec3f m_desiredCamEye;
  Vec3f m_desiredCamTarget;

  bool m_isCamLerping = false;

  // As this value varies from 0..1 we lerp between the original
  //  and desired camera eye/target coords.
  float m_camLerpT = 0;
};
}
