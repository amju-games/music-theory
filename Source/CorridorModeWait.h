// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "CorridorMode.h"

namespace Amju
{
class Tappable;

// * CorridorModeWait *
// Main state: wait for user to tap on a door, scroll left/right, or
//  tap on a tappable.
class CorridorModeWait : public CorridorMode
{
public:
  static const int ID;
  CorridorModeWait();
  virtual void OnActive() override;
  virtual void Update() override;
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual void Reset() override; 

  // Set topic: this will be -1 or 1 more than the final topic index,
  //  if we are exiting a lift.
  void SetTopicOnLevelChange(int newTopic);

private:
  void Drag(bool rightNotLeft);
  void DecelerateScrolling();
  void CheckTappables();
  void ShowTopicName(bool showNotHide);
  void SetCurrentTopic();
  void OnTapDoorOrArch();

  // If true, we are scrolling left or right, and no user interaction is 
  //  allowed until we stop.
  bool IsScrolling() const;

private:
  // To detect swipe left/right, and taps on Tappables
  Vec2f m_touchDownCoord, m_touchUpCoord;
  bool m_isDragging = false;
  bool m_touchDownThisFrame = false;
  bool m_touchUpThisFrame = false;
  bool m_touchDownOnDoor = false;

  bool m_didScroll = false; // true if we scrolled between touch down and touch up
  int m_currentTopicScrolledTo = 0;

  // Time in scrolling mode remaining
  float m_scrollTime = 0;

  Tappable* m_tappedDown = nullptr; // item we tapped down on, or nullptr
};
}

