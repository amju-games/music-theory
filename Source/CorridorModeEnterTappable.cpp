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

  m_isCamLerping = true;
  m_camLerpTime = 0;

  SceneNodeCamera* cam = m_gs->GetCamera();
  m_origCamEye = cam->GetEyePos();
  m_origCamTarget = cam->GetLookAtPos();
}

void CorridorModeEnterTappable::Update()
{
  CorridorMode::Update();

  Tappable* tapped = m_gs->GetSelectedTappable();
  Assert(tapped);
  if (m_isCamLerping)
  {
    SceneNodeCamera* cam = m_gs->GetCamera();

    float dt = TheTimer::Instance()->GetDt();

    // Moving towards tappable camera setting
    m_camLerpTime += dt; // TODO speed
    if (m_camLerpTime > 1)
    {
      // Reached desired cam pos
      // TODO change state
      m_camLerpTime = 1;
      tapped->ActivateGui();

      m_gs->SetMode(CorridorModeShowTappable::ID);
    }

    Vec3f eye = Lerp(m_origCamEye, tapped->GetCameraEyePos(), m_camLerpTime);
    Vec3f look = Lerp(m_origCamTarget, tapped->GetCameraTargetPos(), m_camLerpTime);

    cam->SetEyePos(eye);
    cam->SetLookAtPos(look);
  }
}
}
