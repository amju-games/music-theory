// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase3d.h"
#include <Singleton.h>

namespace Amju
{
class GSMainMenu : public GSBase3d
{
public:
  GSMainMenu();
  virtual void OnActive() override;
  void GoToTopic(int topic);

  // Detect swipes
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

protected:
  void Load3dForTopics();

private:
  // To detect swipe left/right
  Vec2f m_touchDown;
  bool m_isDragging = false;

  bool m_isScrolling = false;
  float m_desiredXPos = 0;
  int m_currentTopicScrolledTo = 0; 
};

typedef Singleton<GSMainMenu> TheGSMainMenu;
}

