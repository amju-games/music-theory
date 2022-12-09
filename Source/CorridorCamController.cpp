// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include "CorridorCamController.h"
#include "GSMainCorridor.h"

namespace Amju
{
void CorridorCamController::SetIsLerping(bool yesLerp)
{
  m_isCamLerping = yesLerp;
}

void CorridorCamController::Update()
{
  if (m_isCamLerping)
  {
    SceneNodeCamera* cam = TheGSMainCorridor::Instance()->GetCamera();

    Vec3f eye = Lerp(m_origCamEye, m_desiredCamEye, m_camLerpT);
    Vec3f look = Lerp(m_origCamTarget, m_desiredCamTarget, m_camLerpT);

    cam->SetEyePos(eye);
    cam->SetLookAtPos(look);
  }
}

void CorridorCamController::SetDesired(const Vec3f& eye, const Vec3f& target)
{
  m_desiredCamEye = eye;
  m_desiredCamTarget = target;

  // TODO Ptr to Camera should be member?
  SceneNodeCamera* cam = TheGSMainCorridor::Instance()->GetCamera();
  m_origCamEye = cam->GetEyePos();
  m_origCamTarget = cam->GetLookAtPos();

  m_isCamLerping = true;
  m_camLerpT = 0;
}

void CorridorCamController::SetDesiredFromTappable(RCPtr<Tappable> tappable)
{
  Assert(tappable);
  SetDesired(tappable->GetCameraEyePos(), tappable->GetCameraTargetPos());
}

void CorridorCamController::SetLerpT(float t)
{
  m_camLerpT = t;
}
}
