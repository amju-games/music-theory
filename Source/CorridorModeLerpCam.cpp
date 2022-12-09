// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Timer.h>
#include "CorridorModeLerpCam.h"
#include "GSMainCorridor.h"

namespace Amju
{
void CorridorModeLerpCam::OnActive()
{
  m_camLerpT = 0;
}

void CorridorModeLerpCam::Update()
{ 
  CorridorMode::Update();
  
  float dt = TheTimer::Instance()->GetDt();
  
  // Moving towards tappable camera setting
  m_camLerpT += dt * m_speed; 

  if (m_camLerpT >= 1)
  { 
    // Reached desired cam pos
    m_camLerpT = 1;
    OnFinishedLerp();
  }
  SetCamLerpT();
}

void CorridorModeLerpCam::SetCamLerpT()
{
  m_gs->GetCameraController().SetLerpT(m_camLerpT);
}

}

