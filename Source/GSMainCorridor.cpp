// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <cmath>
#include <ClipLineSegBox.h>
#include <File.h>
#include <Game.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <GuiDecTranslate.h>
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
#include "CorridorModeEnterStairs.h"
#include "CorridorModeEnterTappable.h"
#include "CorridorModeExitClassroom.h"
#include "CorridorModeExitStairs.h"
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
#include "TutorialIds.h"
#include "TutorialManager.h"
#include "UserProfile.h"
#include "UseVertexColourShader.h"

// TODO TEMP TEST
#include "Gui3dScene.h"
#include "SceneGraph.h"
#include "ParticleEffect2d.h"

namespace
{
const char* LEVEL_KEY = "level";
}

namespace Amju
{
GSMainCorridor::GSMainCorridor()
{
  m_levelNum = TheGameConfigFile::Instance()->GetInt(LEVEL_KEY, 1);
  LoadCourse();

  m_guiFilename = "Gui/gs_main_menu_corridor.txt";
  //m_sceneFilename = "Scene/corridor-scene-" + ToString(m_levelNum) + ".txt";

  m_modes[CorridorModeEnterClassroom::ID] = new CorridorModeEnterClassroom;
  m_modes[CorridorModeEnterStairs::ID] = new CorridorModeEnterStairs;
  m_modes[CorridorModeExitStairs::ID] = new CorridorModeExitStairs;
  m_modes[CorridorModeEnterTappable::ID] = new CorridorModeEnterTappable;
  m_modes[CorridorModeExitClassroom::ID] = new CorridorModeExitClassroom;
  m_modes[CorridorModeExitTappable::ID] = new CorridorModeExitTappable;
  m_modes[CorridorModeShowTappable::ID] = new CorridorModeShowTappable;
  m_modes[CorridorModeWait::ID] = new CorridorModeWait;

  for (auto p : m_modes)
  {
    CorridorMode* cm = p.second;
    cm->SetGameState(this);
  }

  SetMode(CorridorModeWait::ID);
}

void GSMainCorridor::LoadCourse()
{
  // Load the course which this app presents to the user: we only expect there to
  //  be one instance. We could potentially load this depending on config/user choice.
  std::string courseFile = "Course/grade-1-level-" + ToString(m_levelNum) + ".txt.course";
  Course* course = (Course*)TheResourceManager::Instance()->GetRes(courseFile);
  SetCourse(course);
}

bool GSMainCorridor::LoadTappables()
{
  //// Get root node for adding tappables 
  //PSceneNode root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("tappable-root");
  //Assert(root);

  //File f;
  //std::string filename = "Gui/tappables-" + ToString(m_levelNum) + ".txt";
  //if (!f.OpenRead(filename))
  //{
  //  // Plenty of error messages will be displayed, don't worry
  //  return false;
  //}

  //int num = 0;
  //if (!f.GetInteger(&num))
  //{
  //  f.ReportError("Expected num tappables");
  //  return false;
  //}
  //m_tappables.clear();
  //for (int i = 0; i < num; i++)
  //{
  //  RCPtr<Tappable> t = new Tappable;
  //  if (!t->Load(&f))
  //  {
  //    return false;
  //  }
  //  m_tappables.push_back(t);
  //  root->AddChild(t->GetSceneNode());
  //}
  return true;
}

void GSMainCorridor::TriggerCorridorAnim(float desiredX)
{
  m_posInCorridor->SetTranslation(m_posInCorridor->GetTranslation(1), 0);
  m_posInCorridor->SetTranslation(Vec2f(desiredX, 0), 1);
  m_posInCorridorAnimator->ResetAnimation();
}

void GSMainCorridor::Load3dForTopics()
{
  //const float X_OFFSET = -DISTANCE_BETWEEN_DOORS * 0.5f;

  //// Get root node for adding nodes - TODO SHOULDN'T BE CAMERA
  //SceneNode* root = GetCamera();
  //Assert(root);

  // Get user config, so we know which topics have been unlocked.
////  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);
  //auto profile = TheUserProfile();

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  m_doors.clear();

  // Insert root node for corridor sections
  // Find the animation node which controls our position in the corridor
  m_posInCorridorAnimator = dynamic_cast<GuiDecAnimation*>(GetGui()->GetElementByName("scroll-corridor"));
  // Translation node controlled by animator
  m_posInCorridor = dynamic_cast<GuiDecTranslate*>(m_posInCorridorAnimator->GetChild());

  // Child of m_posInCorridor is a stop to prevent animation values cascading further.
  // This descendant is where we should add door sections.
  GuiComposite* addChildren = dynamic_cast<GuiComposite*>(m_posInCorridor->GetElementByName("add-door-sections-to-me"));

  // TODO Add corridor ends

  // Find the last locked door, so we can put it in shadow.
//  int lastUnlocked = 0;
//  bool allUnlocked = true;

  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);
    Assert(topic);

    // 'Offset' position aloong corridor
    GuiDecTranslate* doorPosInCorridor = new GuiDecTranslate;
    doorPosInCorridor->SetTranslation(Vec2f(static_cast<float>(i) * DISTANCE_BETWEEN_DOORS, 0));
    addChildren->AddChild(doorPosInCorridor);
    constexpr bool NOT_LISTENER = false;
    PGuiElement corridorSection = LoadGui("Gui/corridor-section-1.txt", NOT_LISTENER);
    doorPosInCorridor->AddChild(corridorSection);


    // TODO add something to locked topics so we can see it's locked
//    bool unlocked = (i == 0) || profile->IsTopicUnlocked(topic->GetId());

//    if (unlocked)
//    {
//      lastUnlocked = i;
//    }

    // Why can't we do this?
    //   allUnlocked &&= unlocked;
//    allUnlocked = allUnlocked && unlocked;

    // TODO Load from a list of scene files; each one has a locked and
    //  unlocked variety.
    
    //std::string filename = "Scene/corridor-one-door-" + 
    //  ToString(m_levelNum) + ".txt";
    //PSceneNode node = LoadScene(filename);
    //Assert(node);
    //PSceneNode door = node->GetNodeByName("door");
    //m_doors.push_back(door);

    //// Translate this node
    //Matrix m;

    //// Camera is looking down the x axis, so we translate each piece of the 
    ////  corridor in z... TODO make this translation automatically at
    ////  right angles to view vec?
    //m.Translate(Vec3f(0, 0, X_OFFSET + i * -DISTANCE_BETWEEN_DOORS));

    //node->SetLocalTransform(m);

    //root->AddChild(node);
  }

#ifdef YES_SHADOW
  // Position shadow to the right of the last unlocked door.
  if (AllTopicsPassed())
  {
    // If all unlocked, allow player to go up the stairs, so move shadow 
    //  an extra position to the right.
    lastUnlocked++;
  }
 
  PSceneNode shadow = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("corridor-shadow");
  Assert(shadow);
  Matrix m;
  m.Translate(Vec3f(0, 0, X_OFFSET + lastUnlocked * -DISTANCE_BETWEEN_DOORS));
  m *= shadow->GetLocalTransform();
  shadow->SetLocalTransform(m);
#endif // YES_SHADOW

  // Position right-hand stairs to the right of the last door
  //PSceneNode rightStairs = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("right-arch");
  //if (rightStairs)
  //{
  //  Matrix m;
  //  m.Translate(Vec3f(0, 0, X_OFFSET + numTopics * -DISTANCE_BETWEEN_DOORS));
  //  rightStairs->SetLocalTransform(m);
  //}
}

bool GSMainCorridor::IsLevelPassed() const
{
  return true;
}

void GSMainCorridor::OnDeactive()
{
  GSBase::OnDeactive();

  if (m_currentMode)
  {
    m_currentMode->OnDeactive();
  }
  m_currentMode = nullptr;
}

void GSMainCorridor::SetCameraForNewLevel(bool wentUpNotDown)
{
  //// Reset the camera, so when we pull out of the zoom, we are at a different
  ////  position in the new corridor.
  //// If we went up, camera moves to the left end of the new upstairs corridor.
  //// If we went down, camera moves to right-hand end of the new downstairs corridor.

  //// TODO TEMP TEST
  //// These distances should be calculated per-level because they depend on num topics
  //int numTopicsThisLevel = GetCourse()->GetNumTopics();

  //const float LEFT_END = 60.0f;
  //float leftEnd = LEFT_END + DISTANCE_BETWEEN_DOORS / 2;
  //float rightEnd = 
  //  LEFT_END - DISTANCE_BETWEEN_DOORS * (numTopicsThisLevel + 1) + 
  //  DISTANCE_BETWEEN_DOORS / 2;

  //float newZ = wentUpNotDown ? leftEnd : rightEnd;

  //auto cam = GetCamera();
  //Vec3f eye = cam->GetEyePos(); // Currently the camera does not have the zoom distance added
  //Vec3f target = cam->GetLookAtPos();
  //eye.z = newZ;
  //target.z = newZ;
  //// Set new camera positions
  //cam->SetEyePos(eye);
  //cam->SetLookAtPos(target);

  //// Set the controller: we will lerp backwards from 1..0, so desired positions are
  ////  the new camera positions, with the zoom applied.

  //Vec3f camChange(STAIRS_ZOOM_DIST, 0, 0);
  //GetCameraController().SetDesired(eye + camChange, target + camChange);

  //// Set camera pos and current topic in CorridorModeWait
  //// TODO The camera stuff should just live in CameraController.
  //// TODO Also the currently selected topic should just live in here?

  //// TODO Yuck, better to have a member var (if we don't scrap this)
  //CorridorModeWait* cmw = dynamic_cast<CorridorModeWait*>(
  //  m_modes[CorridorModeWait::ID].GetPtr());

  //Assert(cmw);
  //// If we go up, we are on left, so at topic -1. Else we went down,
  ////  and so we are at the right end, so at the last topic + 1.
  //int newTopic = wentUpNotDown ? -1 : numTopicsThisLevel;
  //cmw->SetCurrentPosAndTopic(newZ, newTopic);
}

void GSMainCorridor::SetLevel(int levelNum)
{
  if (levelNum == m_levelNum)
  {
    return;
  }

  bool wentUpNotDown = (levelNum > m_levelNum);

  m_levelNum = levelNum;

  // Persist level number
  auto gcf = TheGameConfigFile::Instance();
  gcf->SetInt(LEVEL_KEY, m_levelNum);
  gcf->Save();

  // Load course for this level
  LoadCourse();

  //// Load 3D for this level
  //// Load classroom doors for each Topic in this level
  //m_sceneFilename = "Scene/corridor-scene-" + ToString(m_levelNum) + ".txt";
  //Reload3d();

  //SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  //m_camera = dynamic_cast<SceneNodeCamera*>(
  //  root->GetNodeByName("camera"));
  //Assert(m_camera);

  Load3dForTopics();

  LoadTappables();

  SetCameraForNewLevel(wentUpNotDown);
}

int GSMainCorridor::GetLevel() const
{
  return m_levelNum;
}

void GSMainCorridor::OnActive()
{
  GSBase::OnActive();

  //SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  //m_camera = dynamic_cast<SceneNodeCamera*>(
  //  root->GetNodeByName("camera"));
  //Assert(m_camera);

  // TODO What happens when we re-enter this state, and we are on e.g. level 2
  Load3dForTopics();
  LoadTappables();

  SetMode(CorridorModeWait::ID);
  ChangeMode();

  GuiButton* shareButton = (GuiButton*)GetElementByName(m_gui, "share-button");
  Assert(shareButton);
  shareButton->SetCommand(Amju::OnShare);

  // First time tutorial message
  // TODO If we need to, subclass this so we can do specific tutorial stuff.
  // Or maybe a special Corridor Mode type would be better.
  // TODO TEMP TEST
//  QueueFirstTimeMsgs({ TUTORIAL_HELLO }, AMJU_EVERY_TIME);

  // Init HUD
//  auto profile = TheUserProfile();
//  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, profile->GetHints());

  // TEST start particle effect
//  Gui3dScene* particleScene = (Gui3dScene*)GetElementByName(m_gui, "particles-test");

  //auto sg = particleScene->GetSceneGraph();
  //Assert(sg);
  //auto sgroot = sg->GetRootNode(SceneGraph::AMJU_OPAQUE);
  //Assert(sgroot);
  //ParticleEffect2d* node = dynamic_cast<ParticleEffect2d*>(sgroot->GetNodeByName("my-particle-node"));
  //Assert(node);
 
  //node->Start();
}

void GSMainCorridor::Draw2d()
{
  // Don't call GSBase::Draw2d(), so Lurk msg is on top

  if (m_gui)
  {
    UseVertexColourShader();
    m_gui->Draw();
  }

  m_currentMode->Draw2d();

  TheLurker::Instance()->Draw();

  DrawDevMenu();
}

void GSMainCorridor::SetMode(int modeId)
{
  if (modeId == m_newModeId)
  {
    return; // right?
  }

std::cout << "Set Mode: new mode ID is " << modeId << "\n";
 
  // Don't change immediately, wait until Update.
  m_newModeId = modeId;
}

void GSMainCorridor::ChangeMode()
{
  if (m_newModeId < 0)
  {
    return;
  }

std::cout << "Change Mode: changing to new mode ID " << m_newModeId << "\n";
  if (m_currentMode)
  {
    m_currentMode->OnDeactive();
  }
  m_currentMode = m_modes[m_newModeId];
  Assert(m_currentMode);
  m_currentMode->OnActive();
  m_newModeId = -1;
}

void GSMainCorridor::GoToTopic()
{
  //if (!IsTopicUnlocked())
  //{
  //  return;
  //}

  SetMode(CorridorModeEnterClassroom::ID);
}

bool GSMainCorridor::AllTopicsPassed() const
{
  auto profile = TheUserProfile();

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();
  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);
    Assert(topic);

    if (!profile->WasTopicEverPassed(topic->GetId()))
    {
      return false;
    }
  } 
  return true;
}

bool GSMainCorridor::IsTopicUnlocked(int topicNum) const
{
  // TODO for dev
////  return true;

  auto profile = TheUserProfile();
////  ConfigFile* config = profile->GetConfigForTopic(KEY_TOPICS);

  Course* course = GetCourse();
  Assert(course);
  if (topicNum >= course->GetNumTopics())
  {
    return false;
  }

  Topic* topic = course->GetTopic(topicNum);
  Assert(topic);

  bool unlocked = 
    (topicNum == 0) ||
    profile->IsTopicUnlocked(topic->GetId());
/////config->Exists(KEY_TOPIC_UNLOCKED + topic->GetId());

  return unlocked;
}

PGuiElement GSMainCorridor::GetDoor()
{
  int currentTopic = TheUserProfile()->GetCurrentTopic();
  Assert(currentTopic >= 0);
  Assert(currentTopic< static_cast<int>(m_doors.size()));
  return m_doors[currentTopic];
}

void GSMainCorridor::Update()
{
  GSBase::Update();

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
  //GetCamera()->Draw();

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
  GSBase::Draw();
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

