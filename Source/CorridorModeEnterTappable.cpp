// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Lerp.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "CorridorModeEnterTappable.h"
#include "CorridorModeShowTappable.h"
#include "GSMainCorridor.h"

namespace Amju
{
const int CorridorModeEnterTappable::ID = 4;

void CorridorModeEnterTappable::OnActive()
{
  CorridorModeLerpCam::OnActive();
  m_gs->GetCameraController().SetTappable(m_gs->GetSelectedTappable());

  // TODO Seen this tappable before?
  // If not, play a "new tappable found" sound effect
  // Maybe add a hint
  // Add to score if there is one, or other rewards
}

void CorridorModeEnterTappable::OnFinishedLerp()
{
  m_gs->SetMode(CorridorModeShowTappable::ID);
}

void CorridorModeEnterTappable::Update()
{
  CorridorModeLerpCam::Update();
}
}
