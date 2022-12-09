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

void CorridorModeEnterStairs::SetNewLevel(int newLevel)
{
  s_newLevel = newLevel;
}

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
std::cout << "CMES::OnFinished!\n";

  auto gsmc = TheGSMainCorridor::Instance();
  gsmc->SetLevel(s_newLevel);

  // TODO Exit stairs state. DO switch camera position in new level.
  m_gs->SetMode(CorridorModeExitStairs::ID);
  m_gs->GetCameraController().SetNoLerp(); // stop lerping camera
}

}
