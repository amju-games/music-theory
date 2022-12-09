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
#include "PlayWav.h"
#include "Tappable.h"

namespace Amju
{
namespace
{
void OnTopic(GuiElement*)
{
  // Topic button for all topics: the currently selected topic is the
  //  one we go to.
//  TheGSMainCorridor::Instance()->GoToTopic();

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
  GuiElement* text = GetElementByName(m_gui, "topic-name-text");
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
    ShowTopicName(false);

    PlayWav(rightNotLeft ? "ok1" : "ok2");
  }
}

void CorridorModeWait::CheckTappables()
{
  if (m_touchDownThisFrame)
  {
    m_touchDownThisFrame = false;
    m_tappedDown = m_gs->TappablePickTest(m_touchDownCoord);
  }

  if (m_touchUpThisFrame)
  {
    m_touchUpThisFrame = false;
    // Check if we picked a Tappable.
    if (m_tappedDown && m_tappedDown == m_gs->TappablePickTest(m_touchDownCoord))
    {
      // Touched down and up on the same Tappable - activate it.
      std::cout << "Tapped on " << m_tappedDown->GetName() << "\n";

      m_gs->OnTapped(m_tappedDown);
    }
  }
}

void CorridorModeWait::OnActive()
{
  CorridorMode::OnActive();

  // Just one topic button, which is fixed in the centre of the screen.
  // The scene scrolls left and right, but sticks so each door is under the 
  //  button.
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "topic-button"));
  Assert(button);
  button->SetCommand(OnTopic);

  SetCurrentTopic();

  m_isScrolling = false;
  m_scrollVel = 0;
  m_isDragging = false;

  SetCamera();
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
    const float MIN_DRAG_DIST = 0.5f; // 1/4 of screen
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
  m_isDragging = mbe.isDown;
  m_touchDownThisFrame = mbe.isDown;
  m_touchUpThisFrame = !mbe.isDown;

  if (mbe.isDown)
  {
    m_touchDownCoord = Vec2f(mbe.x, mbe.y);
  }
  return false;
}
}
