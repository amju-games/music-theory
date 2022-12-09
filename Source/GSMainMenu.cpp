// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <cmath>
#include <ClipLineSegBox.h>
#include <File.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <Lerp.h>
#include <LoadScene.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <SceneGraph.h>
#include <Sign.h>
#include <StringUtils.h>
#include <Timer.h>
#include <Unproject.h>
#include "Consts.h"
#include "CorridorModeEnterClassroom.h"
#include "CorridorModeEnterTappable.h"
#include "CorridorModeExitClassroom.h"
#include "CorridorModeExitTappable.h"
#include "CorridorModeShowTappable.h"
#include "CorridorModeWait.h"
#include "Course.h"
#include "GSAbout.h"
#include "GSMainMenu.h"
#include "GSTitle.h"
#include "Keys.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
namespace
{
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
}

GSMainMenu::GSMainMenu()
{
  m_guiFilename = "Gui/gs_main_menu_corridor.txt";
  m_sceneFilename = "Scene/corridor-scene.txt";

  m_modes[CorridorModeEnterClassroom::ID] = new CorridorModeEnterClassroom;
  m_modes[CorridorModeEnterTappable::ID] = new CorridorModeEnterTappable;
  m_modes[CorridorModeExitClassroom::ID] = new CorridorModeExitClassroom;
  m_modes[CorridorModeExitTappable::ID] = new CorridorModeExitTappable;
  m_modes[CorridorModeShowTappable::ID] = new CorridorModeShowTappable;
  m_modes[CorridorModeWait::ID] = new CorridorModeWait;

  for (CorridorMode* cm : m_modes)
  {
    cm->SetGameState(this);
  }
}

bool GSMainMenu::LoadTappables()
{
  // Get root node for adding tappables - TODO camera?
  SceneNode* root = GetCamera();
  Assert(root);

  File f;
  if (!f.OpenRead("Gui/tappables.txt"))
  {
    return false;
  }
  int num = 0;
  if (!f.GetInteger(&num))
  {
    f.ReportError("Expected num tappables");
    return false;
  }
  m_tappables.clear();
  for (int i = 0; i < num; i++)
  {
    RCPtr<Tappable> t = new Tappable;
    if (!t->Load(&f))
    {
      return false;
    }
    m_tappables.push_back(t);
    root->AddChild(t->GetSceneNode());
  }
  return true;
}

void GSMainMenu::Load3dForTopics()
{
  // Get root node for adding nodes - TODO camera?
  SceneNode* root = GetCamera();
  Assert(root);

  // Get user config, so we know which topics have been unlocked.
  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  m_doors.clear();

  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);

    // TODO add something to locked topics so we can see it's locked
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

    root->AddChild(node);
  }
}

void GSMainMenu::OnDeactive()
{
  GSBase3d::OnDeactive();

  if (m_currentMode)
  {
    m_currentMode->OnDeactive();
  }
  m_currentMode = nullptr;
}

void GSMainMenu::OnActive()
{
  GSBase3d::OnActive();

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  m_camera = dynamic_cast<SceneNodeCamera*>(
    root->GetNodeByName("camera"));
  Assert(m_camera);

  //// Back to title 
  //GuiElement* title = GetElementByName(m_gui, "title-button");
  //title->SetCommand(OnBackToTitle);

  //GuiElement* about = GetElementByName(m_gui, "about-button");
  //about->SetCommand(OnAbout);

  Load3dForTopics();

  // TODO We only need to do this once up front
  LoadTappables();

  SetMode(CorridorModeWait::ID);

  //if (m_currentMode)
  //{
  //  // We have returned from a classroom, so set mode to exiting classroom.
  //  m_currentMode = m_modes[CorridorModeWait::ID]; // TODO
  //}
  //else
  //{
  //  // First time in this state, so set mode to in corridor
  //  m_currentMode = m_modes[CorridorModeWait::ID];
  //}
  //m_currentMode->OnActive();

  // TODO
  //GuiElement* share = GetElementByName(m_gui, "share-button");
  //share->SetCommand(OnShare);

  //GuiElement* newUser = GetElementByName(m_gui, "new-button");
  //newUser->SetCommand(OnNewUser);
}

void GSMainMenu::Draw2d()
{
  GSBase3d::Draw2d();

  m_currentMode->Draw2d();
}

void GSMainMenu::SetMode(int modeId)
{
  std::cout << "Setting mode: " << modeId << "\n";

  if (m_currentMode)
  {
    std::cout << "Deactivating: " << typeid(m_currentMode).name() << "\n";
    m_currentMode->OnDeactive();
  }
  m_currentMode = m_modes[modeId];
  std::cout << "Activating: " << typeid(m_currentMode).name() << "\n";
  m_currentMode->OnActive();
}

void GSMainMenu::GoToTopic()
{
  if (!IsTopicUnlocked())
  {
    return;
  }

  SetMode(CorridorModeEnterClassroom::ID);
}

bool GSMainMenu::IsTopicUnlocked() const
{
  // TODO for dev
  return true;

  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  bool unlocked = 
    (m_currentTopic == 0) ||
    config->Exists(KEY_TOPIC_UNLOCKED + ToString(m_currentTopic));

  return unlocked;
}

void GSMainMenu::SetCurrentTopic(int topicId)
{
  m_currentTopic = topicId;
}

int GSMainMenu::GetCurrentTopic() const
{
  return m_currentTopic;
}

PSceneNode GSMainMenu::GetDoor()
{
  Assert(m_currentTopic >= 0);
  Assert(m_currentTopic< static_cast<int>(m_doors.size()));
  return m_doors[m_currentTopic];
}

void GSMainMenu::Update()
{
  GSBase3d::Update();

  if (m_currentMode)
  {
    m_currentMode->Update();
  }
}

void GSMainMenu::OnTapped(Tappable* tapped)
{
  m_tapped = tapped;
  SetMode(CorridorModeEnterTappable::ID);
}

Tappable* GSMainMenu::GetSelectedTappable()
{
  return m_tapped;
}

Tappable* GSMainMenu::TappablePickTest(const Vec2f& touchCoord)
{
  // Set up camera matrices for Unproject
  GetCamera()->Draw();

  // Get 3D coord at near and far plane for the mouse (touch) coord
  Vec3f nearCoord, farCoord;
  if (   Unproject(touchCoord, 0, &nearCoord)
      && Unproject(touchCoord, 1, &farCoord))
  {
    LineSeg seg(nearCoord, farCoord);

    // Check tappables
    for (auto t : m_tappables)
    {
      // Intersect ray and AABB
      const AABB& aabb = *(t->GetSceneNode()->GetAABB());

      if (Clip(seg, aabb))
      {
        return t;
      }
    }
  }
  return nullptr;
}

void GSMainMenu::Draw()
{
  GSBase3d::Draw();
}

SceneNodeCamera* GSMainMenu::GetCamera()
{
  Assert(m_camera);
  return m_camera;
}

bool GSMainMenu::OnCursorEvent(const CursorEvent& ce)
{
  return m_currentMode->OnCursorEvent(ce);
}

bool GSMainMenu::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return m_currentMode->OnMouseButtonEvent(mbe);
}

}

