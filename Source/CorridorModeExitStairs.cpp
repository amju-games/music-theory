// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "CorridorModeExitStairs.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"

namespace Amju
{
const int CorridorModeExitStairs::ID = 7;

void CorridorModeExitStairs::SetCamLerpT()
{
  m_gs->GetCameraController().SetLerpT(1.0f - m_camLerpT);
}

void CorridorModeExitStairs::OnActive()
{
  CorridorModeLerpCam::OnActive();
}

void CorridorModeExitStairs::OnFinishedLerp()
{
  m_gs->SetMode(CorridorModeWait::ID);
  m_gs->GetCameraController().SetNoLerp(); // stop lerping camera
}

}
