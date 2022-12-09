// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "CorridorModeEnterTappable.h"
#include "CorridorModeShowTappable.h"
#include "GSMainMenu.h"

namespace Amju
{
void CorridorModeEnterTappable::OnActive()
{
  CorridorMode::OnActive();
  m_gs->GetCameraController().SetTappable(m_gs->GetSelectedTappable());
  m_camLerpTime = 0;
}

void CorridorModeEnterTappable::Update()
{
  CorridorMode::Update();

  float dt = TheTimer::Instance()->GetDt();

  // Moving towards tappable camera setting
  m_camLerpTime += dt; // TODO speed
  if (m_camLerpTime > 1)
  {
    // Reached desired cam pos
    m_camLerpTime = 1;
    m_gs->SetMode(CorridorModeShowTappable::ID);
  }
  m_gs->GetCameraController().SetLerpT(m_camLerpTime);
}
}