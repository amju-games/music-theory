// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeEnterStairs.h"
#include "CorridorModeExitStairs.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"
#include "GSShowNewLevelNum.h"
#include "PlayWav.h"

namespace Amju
{
int CorridorModeEnterStairs::s_newLevel = -1;
const int CorridorModeEnterStairs::ID = 6;

void CorridorModeEnterStairs::SetNewLevel(int newLevel)
{
  Assert(newLevel != s_newLevel);

  s_newLevel = newLevel;
}

void CorridorModeEnterStairs::OnActive()
{
  CorridorModeLerpCam::OnActive();

  // TODO Lift noises
  PlayWav(WAV_ENTER_DOOR);
  PlayWav(WAV_DOOR_OPEN);

  // Start animation of lift and zoom in 
  m_gs->StartEnterLiftAnim();
}

void CorridorModeEnterStairs::OnFinishedLerp()
{
  m_gs->SetLevel(s_newLevel);

  // Switch mode to zooming back out
  m_gs->SetMode(CorridorModeExitStairs::ID);

  // Go to mode to show level
    // Go the the topic start state.
  GSShowNewLevelNum* gs = TheGSShowNewLevelNum::Instance();
  gs->SetPrevState(m_gs);
  // No need to queue message, we go now, right?
  GoTo<TheGSShowNewLevelNum>();

}

}
