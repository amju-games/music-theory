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
const int CorridorModeEnterStairs::ID = 6;

static const float ZOOM_DIST = -50.f;

void CorridorModeEnterStairs::SetNewLevel(int newLevel)
{
  Assert(newLevel != s_newLevel);

  s_newLevel = newLevel;
}

void CorridorModeEnterStairs::OnActive()
{
  CorridorModeLerpCam::OnActive();

  auto cam = m_gs->GetCamera();
  Vec3f camChange(ZOOM_DIST, 0, 0);
  Vec3f eye = cam->GetEyePos() + camChange;
  Vec3f target = cam->GetLookAtPos() + camChange;

  m_gs->GetCameraController().SetDesired(eye, target);
}

void CorridorModeEnterStairs::OnFinishedLerp()
{
  m_gs->SetLevel(s_newLevel);

  // Switch mode to zooming back out
  m_gs->SetMode(CorridorModeExitStairs::ID);
}

}
