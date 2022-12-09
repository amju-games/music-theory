// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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
  static const int ID = 0;
  CorridorModeWait();
  virtual void OnActive() override;
  virtual void Update() override;
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

private:
  void Drag(bool rightNotLeft);
  void DecelerateScrolling();
  void CheckTappables();
  void ShowTopicName(bool showNotHide);
  void SetCurrentTopic();

private:
  // To detect swipe left/right, and taps on Tappables
  Vec2f m_touchDownCoord;
  bool m_isDragging = false;
  bool m_touchDownThisFrame = false;
  bool m_touchUpThisFrame = false;

  bool m_isScrolling = false;
  float m_desiredXPos = 0;
  float m_currentXPos = 0;
  float m_scrollVel = 0;
  int m_currentTopicScrolledTo = 0;
 
  Tappable* m_tappedDown = nullptr; // item we tapped down on, or nullptr
};
}

