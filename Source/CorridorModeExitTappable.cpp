// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Timer.h>
#include "Consts.h"
#include "CorridorModeExitTappable.h"
#include "CorridorModeWait.h"
#include "GSMainCorridor.h"
#include "PlayWav.h"

namespace Amju
{
const int CorridorModeExitTappable::ID = 5;

void CorridorModeExitTappable::OnFinishedLerp()
{
  m_gs->SetMode(CorridorModeWait::ID);
  m_gs->GetCameraController().SetNoLerp();
}

void CorridorModeExitTappable::SetCamLerpT()
{
  m_gs->GetCameraController().SetLerpT(1.0f - m_camLerpT);
}

void CorridorModeExitTappable::OnActive()
{
  CorridorModeLerpCam::OnActive();

  PlayWav(WAV_EXIT_TAPPABLE);
}

}
