// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "CorridorModeEnterStairs.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"

namespace Amju
{
const int CorridorModeEnterStairs::ID = UNIQUE_ID;

void CorridorModeEnterStairs::OnActive()
{
std::cout << "CMES::OnActive\n";

  CorridorModeLerpCam::OnActive();

  auto cam = m_gs->GetCamera();
  Vec3f camChange(-50.0f, 0, 0);
  Vec3f eye = cam->GetEyePos() + camChange;
  Vec3f target = cam->GetLookAtPos() + camChange;

  m_gs->GetCameraController().SetDesired(eye, target);
}

void CorridorModeEnterStairs::OnFinishedLerp()
{
std::cout << "CMES::OnFInished!\n";
  m_gs->SetMode(CorridorModeWait::ID);
  m_gs->GetCameraController().SetNoLerp();
}

}
