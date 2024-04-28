// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeExitStairs.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"
#include "PlayWav.h"

namespace Amju
{
const int CorridorModeExitStairs::ID = 7;

void CorridorModeExitStairs::SetCamLerpT()
{
}

void CorridorModeExitStairs::OnActive()
{
  CorridorModeLerpCam::OnActive();

  PlayWav(WAV_ENTER_DOOR);
  PlayWav(WAV_DOOR_OPEN);

  // Start animation of lift doors and zoom out 
  m_gs->StartExitLiftAnim();

}

void CorridorModeExitStairs::OnFinishedLerp()
{
  m_gs->SetMode(CorridorModeWait::ID);
}

}
