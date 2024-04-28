// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeEnterClassroom.h"
#include "GSMainCorridor.h"
#include "GSTopicStart.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
const int CorridorModeEnterClassroom::ID = 1;

void CorridorModeEnterClassroom::OnActive()
{
  std::cout << "Entered CorridorModeEnterClassroom::OnActive OK\n";

  CorridorMode::OnActive();

  PlayWav(WAV_ENTER_DOOR);
  PlayWav(WAV_DOOR_OPEN);

  // Start animation of door and TODO 'camera'.
  auto gsmc = TheGSMainCorridor::Instance();
  gsmc->StartDoorAnim();
  const float animTime = gsmc->GetEnterClassroomAnimTime();

  // Go the the topic start state.
  GSTopicStart* gs = TheGSTopicStart::Instance();
  gs->SetPrevState(m_gs);
  // Time delay so we get to see an animation, e.g. door opening
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSTopicStart>, SecondsFromNow(animTime)));
}
}
