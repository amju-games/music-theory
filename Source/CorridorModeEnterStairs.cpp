// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "CorridorModeEnterStairs.h"
#include "CorridorModeExitStairs.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"

namespace Amju
{
static int s_newLevel = -1;
static bool s_wentUpNotDown = false;

const int CorridorModeEnterStairs::ID = 6;

static const float ZOOM_DIST = -150.f;

void CorridorModeEnterStairs::SetNewLevel(int newLevel)
{
  Assert(newLevel != s_newLevel);

  s_wentUpNotDown = (newLevel > s_newLevel);
  s_newLevel = newLevel;
}

void CorridorModeEnterStairs::OnActive()
{
std::cout << "CMES::OnActive\n";

  CorridorModeLerpCam::OnActive();

  auto cam = m_gs->GetCamera();
  Vec3f camChange(ZOOM_DIST, 0, 0);
  Vec3f eye = cam->GetEyePos() + camChange;
  Vec3f target = cam->GetLookAtPos() + camChange;

  m_gs->GetCameraController().SetDesired(eye, target);
}

void CorridorModeEnterStairs::OnFinishedLerp()
{
std::cout << "CMES::OnFinished!\n";

  auto gsmc = TheGSMainCorridor::Instance();
  gsmc->SetLevel(s_newLevel);

  // Reset the camera, so when we pull out of the zoom, we are at a different
  //  position in the new corridor.
  // If we went up, camera moves to the left end of the new upstairs corridor.
  // If we went down, camera moves to right-hand end of the new downstairs corridor.

  // TODO TEMP TEST
  // These distances should be calculated per-level because they depend on num topics
  float newZ = s_wentUpNotDown ? -740.f : 60.f;

  auto cam = m_gs->GetCamera();
  Vec3f eye = cam->GetEyePos(); // Currently the camera does not have the zoom distance added
  Vec3f target = cam->GetLookAtPos();
  eye.z = newZ;
  target.z = newZ;
  // Set new camera positions
  cam->SetEyePos(eye);
  cam->SetLookAtPos(target);

  // Set the controller: we will lerp backwards from 1..0, so desired positions are
  //  the new camera positions, with the zoom applied.
  Vec3f camChange(ZOOM_DIST, 0, 0);
  m_gs->GetCameraController().SetDesired(eye + camChange, target + camChange);

  // Now switch mode to zooming back out
  m_gs->SetMode(CorridorModeExitStairs::ID);
}

}
