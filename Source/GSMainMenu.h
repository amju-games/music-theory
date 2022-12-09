// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <SceneNode.h>
#include <Singleton.h>
#include "GSBase3d.h"
#include "Tappable.h"

namespace Amju
{
class SceneNodeCamera;

// * GSMainMenu *
// Main menu game state. 
// Displays a scrollable corridor, with doors which lead to individual
//  topics.
// In the corridor are tappable 3D objects, which present the user with 
//  more info.
class GSMainMenu : public GSBase3d
{
public:
  GSMainMenu();
  virtual void OnActive() override;
  virtual void Update() override;
  virtual void Draw() override;

  // Detect swipes
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

  // Conceptually private
  void GoToTopic();

  SceneNodeCamera* GetCamera();

protected:
  void Load3dForTopics();
  bool LoadTappables();
  void Drag(bool rightNotLeft);
  void SetCurrentTopicName();
  void ShowTopicName(bool showNotHide);
  void DecelerateScrolling();
  void UpdateCamera();
  void UpdateOpeningDoor();
  bool IsTopicUnlocked() const;

private:
  // To detect swipe left/right
  Vec2f m_touchDown;
  bool m_isDragging = false;

  bool m_isScrolling = false;
  float m_desiredXPos = 0;
  float m_currentXPos = 0;
  float m_scrollVel = 0;
  int m_currentTopicScrolledTo = 0;

  // Each topic has a door. We open the door for the chosen topic.
  std::vector<PSceneNode> m_doors;
  bool m_doorIsOpening = false;
  float m_doorAngleRads = 0; 

  std::vector<RCPtr<Tappable>> m_tappables;

  // Hold on to the camera scene node, it's useful
  RCPtr<SceneNodeCamera> m_camera;
};

typedef Singleton<GSMainMenu> TheGSMainMenu;
}

