// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <SceneNodeCamera.h>
#include <Sign.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeWait.h"
#include "CorridorModeEnterClassroom.h"
#include "CorridorModeEnterStairs.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "LurkMsg.h"
#include "PlayWav.h"
#include "Tappable.h"
#include "TutorialIds.h"
#include "TutorialManager.h"
#include "UserProfile.h"

namespace Amju
{
namespace
{
// Proportion of screen user must drag to trigger a swipe left/right along
//  the corridor
static const float MIN_DRAG_DIST = 0.25f; // 1/8 of screen
 
} // anon namespace

const int CorridorModeWait::ID = 0;

CorridorModeWait::CorridorModeWait()
{
  m_guiFilename = "Gui/corridor_mode_wait.txt";
}

void CorridorModeWait::Reset()
{
  m_isScrolling = false;
  m_scrollVel = 0;
  m_isDragging = false;
  m_currentTopicScrolledTo = 0;
  m_desiredXPos = 0;
  m_currentXPos = 0; 
  m_scrollVel = 0;
  m_didScroll = false;

  SetCamera();
}

void CorridorModeWait::OnTapDoorOrArch()
{
  auto gsmc = TheGSMainCorridor::Instance();

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  // If topic is out of range, we have tapped on an arch - go to next/prev
  //  level.
  if (m_currentTopicScrolledTo < 0)
  {
    // Go to prev level
    int level = gsmc->GetLevel();
    if (level > 1)
    {
      level--; 
      // Set x pos to FINAL door this level
std::cout << "Go DOWN to level " << level << "\n";

      CorridorModeEnterStairs::SetNewLevel(level);
      gsmc->SetMode(CorridorModeEnterStairs::ID);
    }
    else
    {
std::cout << "At lowest level, so not going down.\n";
    }
    return;
  }

  if (m_currentTopicScrolledTo >= numTopics)
  {
    // Go to next level
    int level = gsmc->GetLevel();
    // TODO Max num levels??
    if (level < 2) // TODO
    {
      level++; 
std::cout << "Go UP to level " << level << "\n";

      CorridorModeEnterStairs::SetNewLevel(level);
      gsmc->SetMode(CorridorModeEnterStairs::ID);
    }
    else
    {
std::cout << "At highest level, so not going up.\n";
    }
    return;
  }

  // If current topic is in range of topics (displayed in this level),
  //  go to Topic Start state.
  if (   TheGame::Instance()->GetState() == gsmc
      && !gsmc->IsTopicUnlocked())
  {
    std::cout << "Topic locked!\n";
    return;
  }

  // Not locked, so go inside the classroom and change state to Topic Start.
  gsmc->SetMode(CorridorModeEnterClassroom::ID);
}

void CorridorModeWait::DecelerateScrolling()
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

void CorridorModeWait::ShowTopicName(bool showNotHide)
{
  GuiElement* text = GetElementByName(m_gui, "topic-name");
  Assert(text);
  text->SetVisible(showNotHide);
}

void CorridorModeWait::SetCurrentTopic()
{
  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  if (   m_currentTopicScrolledTo < 0 
      || m_currentTopicScrolledTo >= numTopics)
  {
    // Past end of Topic doors
    return;
  }

  TheUserProfile()->SetCurrentTopic(m_currentTopicScrolledTo);

  // Set topic name: get topic name...
  Topic* topic = course->GetTopic(m_currentTopicScrolledTo);

  // ...now set the text on screen
  IGuiText* text = dynamic_cast<IGuiText*>(
    GetElementByName(m_gui, "topic-name-text"));
  Assert(text);

  text->SetText(topic->GetDisplayName());
  GuiText* gtext = dynamic_cast<GuiText*>(text);
  if (gtext)
  {
    float w = gtext->CalcSizeToText().x;

    GuiElement* rect = GetElementByName(m_gui, "topic-name-rect");
    Assert(rect);
    Vec2f size = rect->GetSize();
    const float EXTRA = 0.1f;
    size.x = w + EXTRA;
    rect->SetSize(size);

    Vec2f pos = rect->GetLocalPos();
    pos.x = -0.5f * (w + EXTRA);
    rect->SetLocalPos(pos);
  }

  ShowTopicName(m_gs->IsTopicUnlocked());
}

void CorridorModeWait::Drag(bool rightNotLeft)
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
  // We can scroll 1 position to the left and right of the doors, so we can
  //  tap on stairs..?
  if (   ( rightNotLeft && (m_currentTopicScrolledTo >= 0))
      || (!rightNotLeft && (m_currentTopicScrolledTo < numTopics)))
  {
    m_currentTopicScrolledTo -= dir;
    m_desiredXPos += DISTANCE_BETWEEN_DOORS * dir;
    m_scrollVel = MAX_SCROLL_VEL * dir;
    m_isScrolling = true;
    m_didScroll = true;
    ShowTopicName(false);

    PlayWav(rightNotLeft ? WAV_SWIPE_RIGHT : WAV_SWIPE_LEFT);
  }
}

void CorridorModeWait::CheckTappables()
{
  if (m_touchDownThisFrame)
  {
    m_touchDownThisFrame = false;
    m_tappedDown = m_gs->TappablePickTest(m_touchDownCoord);
    std::cout << "Tapped down on: " << (m_tappedDown ? m_tappedDown->GetName() : "nothing") << "\n";
  }
  else if (m_touchUpThisFrame && !m_didScroll)
  {
    m_touchUpThisFrame = false;
    // Check if we picked a Tappable.
    auto tappedUp = m_gs->TappablePickTest(m_touchUpCoord);
    std::cout << "Tapped up on: " << (tappedUp ? tappedUp->GetName() : "nothing") << "\n";

    if (m_tappedDown && m_tappedDown == tappedUp)
    {
      // Touched down and up on the same Tappable - activate it.
      std::cout << "Chosen: " << m_tappedDown->GetName() << "\n";

      m_gs->OnTapped(m_tappedDown);
    }
  }
}

void CorridorModeWait::OnActive()
{
  CorridorMode::OnActive();

  SetCurrentTopic();

  m_isScrolling = false;
  m_scrollVel = 0;
  m_isDragging = false;

  SetCamera();

//  QueueFirstTimeMsgs( { TUTORIAL_HELLO, TUTORIAL_SWIPE }, AMJU_FIRST_TIME_THIS_USER);
}

void CorridorModeWait::SetCamera()
{
  SceneNodeCamera* cam = m_gs->GetCamera();
  Vec3f eye = cam->GetEyePos();
  Vec3f look = cam->GetLookAtPos();
  eye.z = m_currentXPos;
  look.z = m_currentXPos;
  cam->SetEyePos(eye);
  cam->SetLookAtPos(look);
}

void CorridorModeWait::Update()
{
  CorridorMode::Update();

  if (m_isScrolling)
  {
    DecelerateScrolling();

    float dt = TheTimer::Instance()->GetDt();
    m_currentXPos += m_scrollVel * dt;

    if ((m_scrollVel > 0 && m_currentXPos > m_desiredXPos)
      || (m_scrollVel < 0 && m_currentXPos < m_desiredXPos))
    {
      // We have reached the desired position
      m_currentXPos = m_desiredXPos;
      m_scrollVel = 0;
      m_isScrolling = false;

      SetCurrentTopic();

      // Successful swipe - if first time, show more tutorial msgs
//      QueueFirstTimeMsgs( { TUTORIAL_TAP_DOOR }, AMJU_FIRST_TIME_THIS_USER);
    }

    SetCamera();
  }
  else
  {
    // Not scrolling: check if user has selected a Tappable
    CheckTappables();
  }
}

bool CorridorModeWait::OnCursorEvent(const CursorEvent& ce)
{
  if (m_isDragging)
  {
    Vec2f pos = Vec2f(ce.x, ce.y);
    Vec2f dragDist = m_touchDownCoord - pos;
    if (fabs(dragDist.x) > MIN_DRAG_DIST)
    {
      if (dragDist.x < 0)
      {
        Drag(true);
      }
      else
      {
        Drag(false);
      }
      // Reset touch down coord so we don't now start to scroll on every tiny movement
      m_touchDownCoord = pos;
    }
  }
  return false;
}

bool CorridorModeWait::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // Rectangle for tapping door in centre of screen
  const Rect rect(
    DOOR_RECT_XMIN,
    DOOR_RECT_XMAX,
    DOOR_RECT_YMIN,
    DOOR_RECT_YMAX);

  if (m_isScrolling)
  {
    m_touchDownThisFrame = false;
    m_touchUpThisFrame = false;
  }
  else
  {
    m_touchDownThisFrame = mbe.isDown;
    m_touchUpThisFrame = !mbe.isDown;
  }
  
  m_isDragging = mbe.isDown;

  if (mbe.isDown)
  {
    m_touchDownCoord = Vec2f(mbe.x, mbe.y);
    m_didScroll = false;
    m_touchDownOnDoor = rect.IsPointIn(m_touchDownCoord);
  }
  else
  {
    m_touchUpCoord = Vec2f(mbe.x, mbe.y);

    // Check for tap down and up on the door area
    bool touchUpOnDoor = rect.IsPointIn(m_touchUpCoord);
    if (   m_touchDownOnDoor 
        && touchUpOnDoor 
        && !m_isScrolling
    )
        //&& TheTutorialManager::Instance()->MsgHasBeenShown(
        //       TUTORIAL_TAP_DOOR, AMJU_FIRST_TIME_THIS_USER))
    {
      OnTapDoorOrArch();
    }
  }
  return false;
}
}
