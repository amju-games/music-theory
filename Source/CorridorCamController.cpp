// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include "CorridorCamController.h"
#include "GSMainCorridor.h"

namespace Amju
{
void CorridorCamController::Update()
{
  if (m_isCamLerping)
  {
    SceneNodeCamera* cam = TheGSMainCorridor::Instance()->GetCamera();

    Vec3f eye = Lerp(m_origCamEye, m_tappable->GetCameraEyePos(), m_camLerpT);
    Vec3f look = Lerp(m_origCamTarget, m_tappable->GetCameraTargetPos(), m_camLerpT);

    cam->SetEyePos(eye);
    cam->SetLookAtPos(look);
  }
}

void CorridorCamController::SetTappable(RCPtr<Tappable> tappable)
{
  m_tappable = tappable;

  m_isCamLerping = false;
  m_camLerpT = 0;

  if (m_tappable)
  {
    m_isCamLerping = true;
    // TODO Ptr to Camera should be member?
    SceneNodeCamera* cam = TheGSMainCorridor::Instance()->GetCamera();
    m_origCamEye = cam->GetEyePos();
    m_origCamTarget = cam->GetLookAtPos();
  }
}

void CorridorCamController::SetLerpT(float t)
{
  m_camLerpT = t;
}
}
