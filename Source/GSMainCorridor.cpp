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
#include "GSMainCorridor.h"
#include "GSTitle.h"
#include "LurkMsg.h"
#include "MySceneGraph.h"
#include "NumUpdate.h"
#include "PlayWav.h"
#include "UserProfile.h"

namespace Amju
{
GSMainCorridor::GSMainCorridor()
{
  m_guiFilename = "Gui/gs_main_menu_corridor.txt";
  m_sceneFilename = "Scene/corridor-scene-" + ToString(m_levelNum) + ".txt";

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

  SetMode(CorridorModeWait::ID);
}

bool GSMainCorridor::LoadTappables()
{
  // Get root node for adding tappables - TODO camera?
  SceneNode* root = GetCamera();
  Assert(root);

  File f;
  std::string filename = "Gui/tappables-" + ToString(m_levelNum) + ".txt";
  if (!f.OpenRead(filename))
  {
    // Plenty of error messages will be displayed, don't worry
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

void GSMainCorridor::Load3dForTopics()
{
  // Get root node for adding nodes - TODO SHOULDN'T BE CAMERA
  SceneNode* root = GetCamera();
  Assert(root);

  // Get user config, so we know which topics have been unlocked.
  // TODO
  //ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  m_doors.clear();

  for (int i = 0; i < numTopics; i++)
  {
    //Topic* topic = course->GetTopic(i);

    // TODO add something to locked topics so we can see it's locked
    //bool unlocked = (i == 0) || config->Exists(KEY_TOPIC_UNLOCKED + ToString(i));

    // TODO Load from a list of scene files; each one has a locked and
    //  unlocked variety.
    
    std::string filename = "Scene/corridor-one-door-" + 
      ToString(m_levelNum) + ".txt";
    PSceneNode node = LoadScene(filename);
    Assert(node);
    PSceneNode door = node->GetNodeByName("door");
    m_doors.push_back(door);

    // Translate this node
    Matrix m;
    const float X_OFFSET = -DISTANCE_BETWEEN_DOORS * 0.5f;

    // Camera is looking down the x axis, so we translate each piece of the 
    //  corridor in z... TODO make this translation automatically at
    //  right angles to view vec?
    m.Translate(Vec3f(0, 0, X_OFFSET + i * -DISTANCE_BETWEEN_DOORS));

    node->SetLocalTransform(m);

    root->AddChild(node);
  }
}

void GSMainCorridor::OnDeactive()
{
  GSBase3d::OnDeactive();

  if (m_currentMode)
  {
    m_currentMode->OnDeactive();
  }
  m_currentMode = nullptr;
}

void GSMainCorridor::SetLevel(int levelNum)
{
  if (levelNum == m_levelNum)
  {
    return;
  }

  m_levelNum = levelNum;

  // Load course for this level

  // Load 3D for this level
  // Load classroom doors for each Topic in this level
  m_sceneFilename = "Scene/corridor-scene-" + ToString(m_levelNum) + ".txt";
  Reload3d();

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  m_camera = dynamic_cast<SceneNodeCamera*>(
    root->GetNodeByName("camera"));
  Assert(m_camera);

  Load3dForTopics();

  LoadTappables();
}

int GSMainCorridor::GetLevel() const
{
  return m_levelNum;
}

void GSMainCorridor::OnActive()
{
  GSBase3d::OnActive();

  SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  m_camera = dynamic_cast<SceneNodeCamera*>(
    root->GetNodeByName("camera"));
  Assert(m_camera);

  // TODO What happens when we re-enter this state, and we are on e.g. level 2
  Load3dForTopics();
  LoadTappables();

  SetMode(CorridorModeWait::ID);
  ChangeMode();

  GuiButton* shareButton = (GuiButton*)GetElementByName(m_gui, "share-button");
  Assert(shareButton);
  shareButton->SetCommand(Amju::OnShare);

  // Init HUD
  auto profile = TheUserProfile();
  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, profile->GetHints());
}

void GSMainCorridor::Draw2d()
{
  // Don't call GSBase3d::Draw2d(), so Lurk msg is on top

  if (m_gui)
  {
    UseVertexColourShader();
    m_gui->Draw();
  }

  m_currentMode->Draw2d();

  TheLurker::Instance()->Draw();
}

void GSMainCorridor::SetMode(int modeId)
{
  // Don't change immediately, wait until Update.
  m_newModeId = modeId;
}

void GSMainCorridor::ChangeMode()
{
  if (m_newModeId < 0)
  {
    return;
  }

  if (m_currentMode)
  {
    m_currentMode->OnDeactive();
  }
  m_currentMode = m_modes[m_newModeId];
  m_currentMode->OnActive();
  m_newModeId = -1;
}

void GSMainCorridor::GoToTopic()
{
  if (!IsTopicUnlocked())
  {
    return;
  }

  SetMode(CorridorModeEnterClassroom::ID);
}

bool GSMainCorridor::IsTopicUnlocked() const
{
  // TODO for dev
  return true;

  auto profile = TheUserProfile();
  ConfigFile* config = profile->GetConfigForTopic(KEY_TOPICS);
  int currentTopic = profile->GetCurrentTopic();

  bool unlocked = 
    (currentTopic == 0) ||
    config->Exists(KEY_TOPIC_UNLOCKED + ToString(currentTopic));

  return unlocked;
}

PSceneNode GSMainCorridor::GetDoor()
{
  int currentTopic = TheUserProfile()->GetCurrentTopic();
  Assert(currentTopic >= 0);
  Assert(currentTopic< static_cast<int>(m_doors.size()));
  return m_doors[currentTopic];
}

void GSMainCorridor::Update()
{
  GSBase3d::Update();

  ChangeMode(); // if new mode was requested

  if (m_currentMode)
  {
    m_currentMode->Update();
  }

  m_camController.Update();
}

void GSMainCorridor::OnTapped(Tappable* tapped)
{
  m_tapped = tapped;
  // Let the tappable decide what to do
  tapped->OnTapped();
}

Tappable* GSMainCorridor::GetSelectedTappable()
{
  return m_tapped;
}

Tappable* GSMainCorridor::TappablePickTest(const Vec2f& touchCoord)
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

void GSMainCorridor::Draw()
{
  GSBase3d::Draw();
}

SceneNodeCamera* GSMainCorridor::GetCamera()
{
  Assert(m_camera);
  return m_camera;
}

bool GSMainCorridor::OnCursorEvent(const CursorEvent& ce)
{
  return m_currentMode->OnCursorEvent(ce);
}

bool GSMainCorridor::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  return m_currentMode->OnMouseButtonEvent(mbe);
}

CorridorCamController& GSMainCorridor::GetCameraController()
{
  return m_camController;
}
}

