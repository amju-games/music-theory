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

void CorridorModeExitTappable::Update()
{
  CorridorMode::Update();

  float dt = TheTimer::Instance()->GetDt();

  // Moving back away from tappable camera setting
  m_camLerpTime += dt; // TODO speed
  if (m_camLerpTime > 1)
  {
    // Reached desired cam pos
    // TODO change state
    m_camLerpTime = 1;

    m_gs->SetMode(CorridorModeWait::ID);
    m_gs->GetCameraController().SetTappable(nullptr);
  }
  m_gs->GetCameraController().SetLerpT(1.0f - m_camLerpTime);
}

void CorridorModeExitTappable::OnActive()
{
  CorridorMode::OnActive();
  m_camLerpTime = 0;

  PlayWav(WAV_EXIT_TAPPABLE);
}

}
