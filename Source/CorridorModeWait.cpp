// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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
const float MIN_DRAG_DIST = 0.25f; // 1/8 of screen

// TODO Get this dynamically
const int MAX_NUM_LEVELS = 4;
 
} // anon namespace

const int CorridorModeWait::ID = 0;

CorridorModeWait::CorridorModeWait()
{
  m_guiFilename = "Gui/corridor_mode_wait.txt";
}

void CorridorModeWait::SetTopicOnLevelChange(int newTopic)
{
  m_currentTopicScrolledTo = newTopic;
}

void CorridorModeWait::Reset()
{
std::cout << "Call to CorridorModeWait::Reset(), trashing camera!!!\n";

  m_isDragging = false;
  m_currentTopicScrolledTo = 0;
  m_scrollTime = 0;
  m_didScroll = false;
}

void CorridorModeWait::OnTapDoorOrArch()
{
  if (IsScrolling())
  {
    return;
  }

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
      PlayWav(WAV_ENTER_STAIRS);
    }
    else
    {
std::cout << "At lowest level, so not going down.\n";
    }
    return;
  }

  if (   m_currentTopicScrolledTo >= numTopics
      && gsmc->IsLevelPassed())
  {
    // Go to next level
    int level = gsmc->GetLevel();
    // TODO Max num levels??
    if (level < MAX_NUM_LEVELS)
    {
      level++; 
std::cout << "Go UP to level " << level << "\n";

      CorridorModeEnterStairs::SetNewLevel(level);
      gsmc->SetMode(CorridorModeEnterStairs::ID);
      PlayWav(WAV_ENTER_STAIRS);
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
      && !gsmc->IsTopicUnlocked(m_currentTopicScrolledTo))
  {
std::cout << "Topic locked!\n";
return;
  }

  // Not locked, so go inside the classroom and change state to Topic Start.
  gsmc->SetMode(CorridorModeEnterClassroom::ID);
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

  std::cout << "Wait mode: m_currentTopicScrolledTo is: " << m_currentTopicScrolledTo << "\n";

  const bool topicIsOutOfRange = (m_currentTopicScrolledTo < -1) || 
    (m_currentTopicScrolledTo > numTopics);

  const bool noMoreLevels = !GetState()->IsThereALevelAboveCurrentLevel();
  const bool noUpLift = (m_currentTopicScrolledTo == numTopics) && noMoreLevels;

  const int level = GetState()->GetLevel();
  constexpr int LOWEST_LEVEL = 1; // TODO Should we be able to go down to level 0?
  const bool noDownLift = (m_currentTopicScrolledTo == -1) && (level <= LOWEST_LEVEL);

  if (topicIsOutOfRange || noUpLift || noDownLift)
  {
    // Past end of Topic doors
    ShowTopicName(false);
    return;
  }

  // Set text above door or lift

  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);

  // Show "Up" or "Down" for lifts, or topic name for classroom door.
  if (m_currentTopicScrolledTo == -1) 
  {
    Assert(level > LOWEST_LEVEL);
    text->SetText("@@@DOWN");
  }
  else if (m_currentTopicScrolledTo == numTopics)
  {
    Assert(GetState()->IsThereALevelAboveCurrentLevel());
    text->SetText("@@@UP");
  }
  else 
  {
    Assert(m_currentTopicScrolledTo >= 0 &&
      m_currentTopicScrolledTo < numTopics);
    // Set topic name: get topic name, set in GUI text
    TheUserProfile()->SetCurrentTopic(m_currentTopicScrolledTo);
    Topic* topic = course->GetTopic(m_currentTopicScrolledTo);
    text->SetText(topic->GetDisplayName());
  }

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

  ShowTopicName(true);
}

void CorridorModeWait::Drag(bool rightNotLeft)
{
  static auto gsmc = TheGSMainCorridor::Instance();

  if (IsScrolling())
  {
    return;
  }

  Course* course = GetCourse();
  Assert(course);
  int numTopics = course->GetNumTopics();

  const int dir = rightNotLeft ? 1 : -1; // direction

  // Check for end of corridor
  // We can scroll 1 position to the left and right of the doors, so we can
  //  tap on stairs.
  // We can't scroll past locked topics.
  bool canSwipe = false;
  // We can scroll left (player swiped RIGHT though) if there is a door or
  //  stairwell/arch to the left.
  if (rightNotLeft && (m_currentTopicScrolledTo >= 0))
  {
    canSwipe = true;
  }
  // We can scroll right if there is an door to the right...
  if (!rightNotLeft)
  {
    // ...and it's unlocked...
    if (   gsmc->IsTopicUnlocked(m_currentTopicScrolledTo + 1)
        && (m_currentTopicScrolledTo < numTopics))
    {
      canSwipe = true;
    }
    // ...or ALL the topics are passed
    if (   gsmc->AllTopicsPassed()
        && (m_currentTopicScrolledTo < numTopics))
    {
      canSwipe = true;
    }
  }

  if (canSwipe)
  {
    // Update current topic immediately; on screen, we scroll to the new position.
    m_currentTopicScrolledTo -= dir;

    // Set change in X, giving new X to which we animate 
    float xChange = DISTANCE_BETWEEN_DOORS * dir;
    GetState()->TriggerCorridorAnim(xChange);

    // Wait until animation completes
    m_scrollTime = GetState()->GetEnterClassroomAnimTime(); // time we are in 'scroll mode'
    m_isDragging = false;

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

  m_scrollTime = 0;
  m_isDragging = false;

//  QueueFirstTimeMsgs( { TUTORIAL_HELLO, TUTORIAL_SWIPE }, AMJU_FIRST_TIME_THIS_USER);
}

bool CorridorModeWait::IsScrolling() const
{
  return m_scrollTime > 0;
}

void CorridorModeWait::Update()
{
  CorridorMode::Update();

  if (IsScrolling())
  {
    float dt = TheTimer::Instance()->GetDt();
    m_scrollTime -= dt;

    if (m_scrollTime < 0)
    {
      m_scrollTime = 0;

      SetCurrentTopic();

      // Successful swipe - if first time, show more tutorial msgs
//      QueueFirstTimeMsgs( { TUTORIAL_TAP_DOOR }, AMJU_FIRST_TIME_THIS_USER);
    }
  }
  else
  {
    // Not scrolling: check if user has selected a Tappable
    CheckTappables();
  }
}

bool CorridorModeWait::OnCursorEvent(const CursorEvent& ce)
{
  if (IsScrolling())
  {
    return false;
  }

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
  if (IsScrolling())
  {
    m_touchDownThisFrame = false;
    m_touchUpThisFrame = false;
    return true; // don't allow any user input until we stop
  }

  // Rectangle for tapping door in centre of screen
  const Rect rect(
    DOOR_RECT_XMIN,
    DOOR_RECT_XMAX,
    DOOR_RECT_YMIN,
    DOOR_RECT_YMAX);

  m_touchDownThisFrame = mbe.isDown;
  m_touchUpThisFrame = !mbe.isDown;
  
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
        && !IsScrolling()
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
