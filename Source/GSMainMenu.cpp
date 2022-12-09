// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <cmath>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <LoadScene.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <SceneGraph.h>
#include <Sign.h>
#include <StringUtils.h>
#include <Timer.h>
#include "Course.h"
#include "GSAbout.h"
#include "GSMainMenu.h"
#include "GSTitle.h"
#include "GSTopicStart.h"
#include "Keys.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
namespace
{
const float DISTANCE_BETWEEN_DOORS = 135;
const float MAX_SCROLL_VEL = 300.0f;
const float SCROLL_DECEL = 280.0f;
const float DOOR_OPEN_ROT_VEL = 1.0f; // rad/sec
const float MAX_DOOR_ANGLE = 2.0f;
const float CAMERA_START = 100.0f; 
const float CAM_ZOOM_MULT = 20.0f;

void OnShare(GuiElement*)
{
  //  TheGSMainMenu::Instance()->HideButtons()->ScrollUp();
}

void OnNewUser(GuiElement*)
{
  //  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
}

void OnAbout(GuiElement*)
{
  //  TheGSMainMenu::Instance()->HideButtons()->ScrollDown();
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSAbout>, SecondsFromNow(1.0f)));
}

void OnBackToTitle(GuiElement*)
{
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

void OnTopic(GuiElement*)
{
  // Topic button for all topics: the currently selected topic is the
  //  one we go to.
  TheGSMainMenu::Instance()->GoToTopic();
}
}

GSMainMenu::GSMainMenu()
{
  m_guiFilename = "Gui/gs_main_menu_corridor.txt";
  m_sceneFilename = "Scene/corridor-scene.txt";
}

void GSMainMenu::Load3dForTopics()
{
  // Get user config, so we know which topics have been unlocked.
  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  SceneNode* camera = root->GetNodeByName("camera");
  Assert(camera);

  m_doors.clear();

  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);
    bool unlocked = (i == 0) || config->Exists(KEY_TOPIC_UNLOCKED + ToString(i));

    // TODO Load from a list of scene files; each one has a locked and
    //  unlocked variety.
    PSceneNode node = LoadScene("Scene/corridor-one-door.txt");
    Assert(node);
    node->GetNodeByName("floor-and-wall")->SetIsLit(true);

    PSceneNode door = node->GetNodeByName("door");
    door->SetIsLit(true);
    m_doors.push_back(door);

    // Translate this node
    Matrix m;
    const float X_OFFSET = -DISTANCE_BETWEEN_DOORS * 0.5f;

    //m.Scale(0.5f, 0.5f, 0.5f);
    // Camera is looking down the x axis, so we translate each piece of the 
    //  corridor in z... TODO make this translation automatically at
    //  right angles to view vec?
    m.Translate(Vec3f(0, 0, X_OFFSET + i * -DISTANCE_BETWEEN_DOORS));

    node->SetLocalTransform(m);

    camera->AddChild(node);
  }
}

void GSMainMenu::OnActive()
{
  // Load the GUI. The buttons here correspond to Topics in the Course, but
  //  we don't want to try to programatically add the buttons. So the GUI layout
  //  here is coupled to the Course definition.
  GSBase3d::OnActive();

  // Just one topic button, which is fixed in the centre of the screen.
  // The scene scrolls left and right, but sticks so each door is under the 
  //  button.
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "topic-button"));
  Assert(button);
  button->SetCommand(OnTopic); 

  // Back to title 
  GuiElement* title = GetElementByName(m_gui, "title-button");
  title->SetCommand(OnBackToTitle);

  GuiElement* about = GetElementByName(m_gui, "about-button");
  about->SetCommand(OnAbout);

  Load3dForTopics();

  SetCurrentTopicName();

  m_doorIsOpening = false;
  m_doorAngleRads = 0;

  // TODO
  //GuiElement* share = GetElementByName(m_gui, "share-button");
  //share->SetCommand(OnShare);

  //GuiElement* newUser = GetElementByName(m_gui, "new-button");
  //newUser->SetCommand(OnNewUser);
}

void GSMainMenu::UpdateOpeningDoor()
{
  if (m_doorIsOpening)
  {
    float dt = TheTimer::Instance()->GetDt();
    float rot = DOOR_OPEN_ROT_VEL * dt;

    Assert(m_currentTopicScrolledTo >= 0);
    Assert(m_currentTopicScrolledTo < static_cast<int>(m_doors.size()));
    PSceneNode door = m_doors[m_currentTopicScrolledTo];
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

void GSMainMenu::GoToTopic()
{
  m_doorIsOpening = true;
  PlayWav("doorcreak");

  // Go the the topic start state.
  GSTopicStart* gs = TheGSTopicStart::Instance();
  gs->SetTopic(m_currentTopicScrolledTo);
  gs->SetPrevState(this);
  // Time delay so we get to see an animation, e.g. door opening
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSTopicStart>, SecondsFromNow(2.0f)));

  HideButtons();
}

void GSMainMenu::Drag(bool rightNotLeft)
{
  if (m_isScrolling)
  {
    return;
  }

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  int dir = rightNotLeft ? 1 : -1; // direction

  // Check for end of corridor
  if (   ( rightNotLeft && (m_currentTopicScrolledTo > 0))
      || (!rightNotLeft && (m_currentTopicScrolledTo < numTopics - 1)))
  {
    m_currentTopicScrolledTo -= dir;
    m_desiredXPos += DISTANCE_BETWEEN_DOORS * dir;
    m_scrollVel = MAX_SCROLL_VEL * dir;
    m_isScrolling = true;
    ShowTopicName(false);
  }
}

void GSMainMenu::ShowTopicName(bool showNotHide)
{
  GuiElement* text = GetElementByName(m_gui, "topic-name-text");
  Assert(text);
  text->SetVisible(showNotHide);
}

void GSMainMenu::SetCurrentTopicName()
{
  // Set topic name: get topic name...
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(m_currentTopicScrolledTo);

  // ...now set the text on screen
  IGuiText* text = dynamic_cast<IGuiText*>(
    GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(topic->GetDisplayName());

  ShowTopicName(true);
}

void GSMainMenu::DecelerateScrolling()
{
  float dt = TheTimer::Instance()->GetDt();

  float sign = Sign(m_scrollVel);
  float origScrollVel = m_scrollVel;
  m_scrollVel -= SCROLL_DECEL * sign * dt;
  if (Sign(m_scrollVel) != sign)
  {
    // Sign changed! We don't want to reverse direction, so restore old value.
    m_scrollVel = origScrollVel;
  }
}

void GSMainMenu::UpdateCamera()
{
  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  SceneNodeCamera* camera = dynamic_cast<SceneNodeCamera*>(
    root->GetNodeByName("camera"));
  Assert(camera);
  Vec3f eye = camera->GetEyePos();
  Vec3f look = camera->GetLookAtPos();
  eye.z = m_currentXPos;
  look.z = m_currentXPos;

  eye.x = CAMERA_START - m_doorAngleRads * m_doorAngleRads * CAM_ZOOM_MULT;
  look.x = eye.x - 10.0f;

  camera->SetEyePos(eye);
  camera->SetLookAtPos(look);
}

void GSMainMenu::Update()
{
  GSBase3d::Update();
  
  if (m_isScrolling)
  {
    DecelerateScrolling();

    float dt = TheTimer::Instance()->GetDt();
    m_currentXPos += m_scrollVel * dt;

    if (   (m_scrollVel > 0 && m_currentXPos > m_desiredXPos)
        || (m_scrollVel < 0 && m_currentXPos < m_desiredXPos))
    {
      // We have reached the desired position
      m_currentXPos = m_desiredXPos;
      m_scrollVel = 0;
      m_isScrolling = false;

      SetCurrentTopicName();
    }
  }

  UpdateCamera();

  UpdateOpeningDoor();
}

bool GSMainMenu::OnCursorEvent(const CursorEvent& ce)
{
  if (m_isDragging)
  {
    Vec2f pos = Vec2f(ce.x, ce.y);
    Vec2f dragDist = m_touchDown - pos;
    const float MIN_DRAG_DIST = 0.5f; // 1/4 of screen
    if (fabs(dragDist.x) > MIN_DRAG_DIST)
    {
      if (dragDist.x < 0)
      {
        std::cout << "Drag right!\n";
        Drag(true);
      }
      else
      {
        std::cout << "Drag left!\n";
        Drag(false);
      }
      m_touchDown = pos;
    }
  }
  return false;
}

bool GSMainMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  m_isDragging = mbe.isDown;
  if (mbe.isDown)
  {
    m_touchDown = Vec2f(mbe.x, mbe.y);
  }
  return false;
}

}

