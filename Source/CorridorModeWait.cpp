// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <SceneNodeCamera.h>
#include <Sign.h>
#include <Timer.h>
#include "Consts.h"
#include "CorridorModeWait.h"
#include "CorridorModeEnterClassroom.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "LurkMsg.h"
#include "PlayWav.h"
#include "Tappable.h"
#include "TutorialIds.h"
#include "TutorialManager.h"

namespace Amju
{
namespace
{
// Proportion of screen user must drag to trigger a swipe left/right along
//  the corridor
static const float MIN_DRAG_DIST = 0.25f; // 1/8 of screen
 
void OnTopic(GuiElement*)
{
  if (!TheGSMainCorridor::Instance()->IsTopicUnlocked())
  {
    std::cout << "Topic locked!\n";
    return;
  }

  TheGSMainCorridor::Instance()->SetMode(CorridorModeEnterClassroom::ID);
}
} // anon namespace

CorridorModeWait::CorridorModeWait()
{
  m_guiFilename = "Gui/corridor_mode_wait.txt";
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
  m_gs->SetCurrentTopic(m_currentTopicScrolledTo);

  // Set topic name: get topic name...
  Course* course = GetCourse();
  Assert(course);
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
  if (   ( rightNotLeft && (m_currentTopicScrolledTo > 0))
      || (!rightNotLeft && (m_currentTopicScrolledTo < numTopics - 1)))
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

  QueueFirstTimeMsgs( { TUTORIAL_HELLO, TUTORIAL_SWIPE }, AMJU_FIRST_TIME_THIS_USER);
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
      QueueFirstTimeMsgs( { TUTORIAL_TAP_DOOR }, AMJU_FIRST_TIME_THIS_USER);
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

  std::cout << "Mouse " << (mbe.isDown ? "DOWN" : "UP") << " event\n";

  if (m_isScrolling)
  {
    std::cout << "Scrolling\n";
    m_touchDownThisFrame = false;
    m_touchUpThisFrame = false;
  }
  else
  {
    std::cout << "NOT Scrolling\n";
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
        && TheTutorialManager::Instance()->MsgHasBeenShown(
               TUTORIAL_TAP_DOOR, AMJU_FIRST_TIME_THIS_USER))
    {
      OnTopic(nullptr); // replaces GuiButton
    }
  }
  return false;
}
}
