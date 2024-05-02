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
constexpr bool NOT_LISTENER = false;
constexpr float MAX_ZOOM = 3.f;
constexpr float MIN_ZOOM = 1.f;
constexpr float ZOOM_VEL = 2.f;
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

float GSMainCorridor::GetEnterClassroomAnimTime() const
{
  return .5f;
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

void GSMainCorridor::TriggerCorridorAnim(float desiredXChange)
{
  std::cout << "TriggerCorridorAnim: desiredXChange: " << desiredXChange << "\n";

  // Current position
  Vec2f currentPos = m_posInCorridor->GetTranslation(1);
  m_posInCorridor->SetTranslation(currentPos, 0);

  // Desired position
  m_posInCorridor->SetTranslation(currentPos + Vec2f(desiredXChange, 0), 1);

  // Kick off the animation
  m_posInCorridorAnimator->SetCycleTime(GetEnterClassroomAnimTime());
  m_posInCorridorAnimator->ResetAnimation();
}

void GSMainCorridor::PauseAnimsOnSection(PGuiElement corridorSection)
{
  GuiElement* doorAnimElem = corridorSection->GetElementByName("door-anim");
  GuiDecAnimation* doorAnim = dynamic_cast<GuiDecAnimation*>(doorAnimElem);
  if (doorAnim)
  {
    doorAnim->SetIsPaused(true);
  }
}

void GSMainCorridor::StartExitLiftAnim()
{
  StartEnterLiftAnim();
  m_isZooming = Zoom::ZOOM_OUT;
  m_zoom = MAX_ZOOM;
}

void GSMainCorridor::StartEnterLiftAnim()
{
  for (auto& elem : m_animatableLiftSections)
  {
    GuiElement* doorAnimElem = elem->GetElementByName("door-anim");
    GuiDecAnimation* doorAnim = dynamic_cast<GuiDecAnimation*>(doorAnimElem);
    if (doorAnim)
    {
      doorAnim->ResetAnimation();
      doorAnim->SetIsPaused(false);
    }
  }

  m_isZooming = Zoom::ZOOM_IN;
  m_zoom = MIN_ZOOM;
}

void GSMainCorridor::StartDoorAnim()
{
  auto corridorSection = GetCurrentCorridorSection();

  GuiElement* doorAnimElem = corridorSection->GetElementByName("door-anim");
  GuiDecAnimation* doorAnim = dynamic_cast<GuiDecAnimation*>(doorAnimElem);
  if (doorAnim)
  {
    doorAnim->ResetAnimation();
    doorAnim->SetIsPaused(false);
  }

  m_isZooming = Zoom::ZOOM_IN;
  m_zoom = MIN_ZOOM;
}

void GSMainCorridor::AddOneBlankSection(int sectionNum, GuiComposite* addChildren)
{
  PGuiElement corridorSection = LoadGui("Gui/corridor-end-section.txt", NOT_LISTENER);
  addChildren->AddChild(corridorSection);
  corridorSection->SetLocalPos(Vec2f(static_cast<float>(sectionNum) * DISTANCE_BETWEEN_DOORS, 0));
}

void GSMainCorridor::AddOneLiftSection(int sectionNum, GuiComposite* addChildren)
{
  PGuiElement liftSection = LoadGui("Gui/corridor-lift-section.txt", NOT_LISTENER);
  m_animatableLiftSections.push_back(liftSection); 
  addChildren->AddChild(liftSection);
  liftSection->SetLocalPos(Vec2f(sectionNum * DISTANCE_BETWEEN_DOORS, 0));

  // Prevent animations from immediately starting
  PauseAnimsOnSection(liftSection);
}

bool GSMainCorridor::IsThereALevelAboveCurrentLevel() const
{
  // TODO We need a list of courses, so we know when we are at the EOC
  return GetLevel() < 4;  
}

void GSMainCorridor::AddRightLiftOrBlankSection(int sectionNum, GuiComposite* addChildren)
{
  bool hasLiftToNextLevel = IsThereALevelAboveCurrentLevel();

  // Lift to next level -- TODO diff styles?
  if (hasLiftToNextLevel) 
  {
    AddOneLiftSection(sectionNum, addChildren);
  }
  else
  {
    // No more levels: just add a black section
    AddOneBlankSection(sectionNum, addChildren);
  }
  // Extra one at far end
  AddOneBlankSection(sectionNum + 1, addChildren);
}

void GSMainCorridor::AddLeftLiftOrBlankSection(GuiComposite* addChildren)
{
  AddOneBlankSection(-2, addChildren);

  if (m_levelNum == 1)
  {
    // Add corridor left end: add two of these?
    AddOneBlankSection(-1, addChildren);
  }
  else
  {
    // Add LIFT if level > 1: you can do down in this lift to the previous level.
    AddOneLiftSection(-1, addChildren);
  }
}

void GSMainCorridor::AddCorridorSection(int sectionNum, GuiComposite* addChildren)
{
  // TODO list of names in topic file?
  // Just cycle through some filenames, but set colours on walls and doors
  //  programatically.
  PGuiElement corridorSection = LoadGui("Gui/corridor-section-1.txt", NOT_LISTENER);
  // Find door anim element, store it so we can open the correct door when it's selected.
  m_animatableCorridorSections.push_back(corridorSection);

  // Prevent animations from immediately starting
  PauseAnimsOnSection(corridorSection);

  addChildren->AddChild(corridorSection);
  // Offset position along corridor
  corridorSection->SetLocalPos(Vec2f(static_cast<float>(sectionNum) * DISTANCE_BETWEEN_DOORS, 0));

  // TODO add something to locked topics so we can see it's locked
}

void GSMainCorridor::LoadCorridor()
{
  // Get user config, so we know which topics have been unlocked.
////  ConfigFile* config = TheUserProfile()->GetConfigForTopic(KEY_TOPICS);
  //auto profile = TheUserProfile();

  m_zoom = 1.f; 
  m_isZooming = Zoom::NO_ZOOM;

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  // Trash and reload the GUI
  m_gui = LoadGui(m_guiFilename);

  m_animatableCorridorSections.clear();
  m_animatableLiftSections.clear();

  // Insert root node for corridor sections
  // Find the animation node which controls our position in the corridor
  m_posInCorridorAnimator = dynamic_cast<GuiDecAnimation*>(GetGui()->GetElementByName("scroll-anim"));

  // Translation node controlled by animator
  m_posInCorridor = dynamic_cast<GuiDecTranslate*>(m_posInCorridorAnimator->GetChild());

  // This descendant is where we should add door sections.
  GuiComposite* addChildren = dynamic_cast<GuiComposite*>(m_posInCorridor->GetElementByName("add-door-sections-to-me"));

  AddLeftLiftOrBlankSection(addChildren);

  for (int i = 0; i < numTopics; i++)
  {
    Topic* topic = course->GetTopic(i);
    Assert(topic);

    AddCorridorSection(i, addChildren);
  }

  AddRightLiftOrBlankSection(numTopics, addChildren);
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
  m_lastXPosInCorridor = m_posInCorridor->GetLocalPos().x;
}

void GSMainCorridor::SetLevel(int levelNum)
{
  if (levelNum == m_levelNum)
  {
    return;
  }

  bool wentUpNotDown = (levelNum > m_levelNum);

  std::cout << "Setting new level to: " << levelNum << ", was: " << m_levelNum << "\n";

  m_levelNum = levelNum;

  // Persist level number
  auto gcf = TheGameConfigFile::Instance();
  gcf->SetInt(LEVEL_KEY, m_levelNum);
  gcf->Save();

  // Load course for this level
  LoadCourse();

  // TODO Also set last topic to 0 or the end? 
  LoadCorridor();

  LoadTappables();

  CorridorModeWait* waitMode = dynamic_cast<CorridorModeWait*>(m_modes[CorridorModeWait::ID].GetPtr());
  int topicNum = -1;
  if (!wentUpNotDown)
  {
    topicNum = GetCourse()->GetNumTopics();
  }
  m_lastXPosInCorridor = -(DISTANCE_BETWEEN_DOORS * static_cast<float>(topicNum)); // Negate!
  waitMode->SetTopicOnLevelChange(topicNum);
  SetCorridorXPosition();
  m_posInCorridorAnimator->ResetAnimation();
//  m_posInCorridorAnimator->SetIsPaused(true);
}

int GSMainCorridor::GetLevel() const
{
  return m_levelNum;
}

void GSMainCorridor::SetCorridorXPosition()
{
  m_posInCorridor->SetTranslation(Vec2f(m_lastXPosInCorridor, 0), 0);
  m_posInCorridor->SetTranslation(Vec2f(m_lastXPosInCorridor, 0), 1);
}

void GSMainCorridor::OnActive()
{
  GSBase::OnActive();

  //SceneNode* root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  //m_camera = dynamic_cast<SceneNodeCamera*>(
  //  root->GetNodeByName("camera"));
  //Assert(m_camera);

  // TODO What happens when we re-enter this state, and we are on e.g. level 2

  // TODO Surely we shouldn't do this every time we re-activate, we will be on the same
  //  level as before, 99% of the time.
  LoadCorridor();

  // Set position in corridor
  SetCorridorXPosition();

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
}

void GSMainCorridor::Draw2d()
{
  // Don't call GSBase::Draw2d(), so Lurk msg is on top

  if (m_gui)
  {
    UseVertexColourShader();

    AmjuGL::PushMatrix();
    AmjuGL::Scale(m_zoom, m_zoom, 1.f);
    m_gui->Draw();
    AmjuGL::PopMatrix();
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

void GSMainCorridor::EnterClassroomForCurrentTopic()
{
  //if (!IsTopicUnlocked())
  //{
  //  return;
  //}

  SetMode(CorridorModeEnterClassroom::ID);
}

bool GSMainCorridor::AllTopicsPassed() const
{
  return true; // for now, allow player to see all topics

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
  return true;

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

PGuiElement GSMainCorridor::GetCurrentCorridorSection()
{
  // TODO Also allow for corridor sections with no associated topic
  int currentTopic = TheUserProfile()->GetCurrentTopic();
  Assert(currentTopic >= 0);
  Assert(currentTopic < static_cast<int>(m_animatableCorridorSections.size()));
  return m_animatableCorridorSections[currentTopic];
}

void GSMainCorridor::Update()
{
  GSBase::Update();

  ChangeMode(); // if new mode was requested

  if (m_currentMode)
  {
    m_currentMode->Update();
  }

  UpdateZoom();
}

void GSMainCorridor::UpdateZoom()
{
  [[maybe_unused]] const float dt = TheTimer::Instance()->GetDt();
  if (m_isZooming == Zoom::ZOOM_IN)
  {
    m_zoom += ZOOM_VEL * dt;
    if (m_zoom > MAX_ZOOM)
    {
      std::cout << "Finished zooming in!\n";
      m_isZooming = Zoom::NO_ZOOM;
      m_zoom = MAX_ZOOM;
      // TODO Notify?
    }
  }
  else if (m_isZooming == Zoom::ZOOM_OUT)
  {
    m_zoom -= ZOOM_VEL * dt;
    if (m_zoom < MIN_ZOOM)
    {
      std::cout << "Finished zooming out!\n";
      m_isZooming = Zoom::NO_ZOOM;
      m_zoom = MIN_ZOOM;
      // TODO Notify?
    }
  }
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
}

