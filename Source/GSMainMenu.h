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
  Tappable* GetTapped();
  void CheckTappables();

private:
  // To detect swipe left/right, and taps on Tappables
  Vec2f m_touchDown;
  bool m_isDragging = false;
  bool m_touchDownThisFrame = false;
  bool m_touchUpThisFrame = false;

  bool m_isScrolling = false;
  float m_desiredXPos = 0;
  float m_currentXPos = 0;
  float m_scrollVel = 0;
  int m_currentTopicScrolledTo = 0;

  // Each topic has a door. We open the door for the chosen topic.
  std::vector<PSceneNode> m_doors;
  bool m_doorIsOpening = false;
  float m_doorAngleRads = 0; 

  // Items which respond to being tapped, to show some kind of info.
  std::vector<RCPtr<Tappable>> m_tappables;
  Tappable* m_tappedDown = nullptr; // item we tapped down on, or nullptr

  // Hold on to the camera scene node, it's useful
  RCPtr<SceneNodeCamera> m_camera;
  // Store camera info for when we interpolate to tappable camera
  Vec3f m_camEye;
  Vec3f m_camTarget;
  bool m_isCamLerping = false;
  float m_camLerpTime = 0;
};

typedef Singleton<GSMainMenu> TheGSMainMenu;
}

