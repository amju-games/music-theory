// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <MessageQueue.h>
#include <SceneNodeCamera.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeEnterClassroom.h"
#include "GSMainMenu.h"
#include "GSTopicStart.h"
#include "PlayWav.h"

namespace Amju
{
void CorridorModeEnterClassroom::UpdateOpeningDoor()
{
  if (m_doorIsOpening)
  {
    float dt = TheTimer::Instance()->GetDt();
    float rot = DOOR_OPEN_ROT_VEL * dt;

    PSceneNode door = m_gs->GetDoor();
    Matrix tr = door->GetLocalTransform();
    Matrix m;
    m.RotateY(rot);
    door->SetLocalTransform(m * tr);

    m_doorAngleRads += rot;
    if (m_doorAngleRads > MAX_DOOR_ANGLE)
    {
      m_doorAngleRads = MAX_DOOR_ANGLE;
      m_doorIsOpening = false;
    }
  }
}

void CorridorModeEnterClassroom::Update()
{
  CorridorMode::Update();

  UpdateOpeningDoor();

  // Camera zoom through door
  if (m_doorAngleRads != 0)
  {
    SceneNodeCamera* cam = m_gs->GetCamera();
    Vec3f eye = cam->GetEyePos();
    Vec3f look = cam->GetLookAtPos();

    eye.x = CAMERA_START - m_doorAngleRads * m_doorAngleRads * CAM_ZOOM_MULT;
    look.x = eye.x - 10.0f;

    cam->SetEyePos(eye);
    cam->SetLookAtPos(look);
  }
}

void CorridorModeEnterClassroom::OnActive()
{
  std::cout << "Entered CorridorModeEnterClassroom::OnActive OK\n";

  CorridorMode::OnActive();

//  m_gs->HideButtons();

  m_doorAngleRads = 0;
  m_doorIsOpening = true;
  PlayWav("doorcreak");


  // Go the the topic start state.
  GSTopicStart* gs = TheGSTopicStart::Instance();
  gs->SetTopic(m_gs->GetCurrentTopic());
  gs->SetPrevState(m_gs);
  // Time delay so we get to see an animation, e.g. door opening
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSTopicStart>, SecondsFromNow(2.0f)));

  std::cout << "Exited CorridorModeEnterClassroom::OnActive OK\n";

}
}