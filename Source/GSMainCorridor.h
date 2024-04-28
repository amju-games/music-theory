// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <map>
#include <SceneNodeCamera.h>
#include <Singleton.h>
#include "CorridorCamController.h"
#include "CorridorMode.h"
#include "GSBase3d.h"
#include "Tappable.h"

namespace Amju
{
class GuiComposite;
class GuiDecAnimation;
class GuiDecTranslate;

// * GSMainCorridor *
// Main menu game state. 
// Displays a scrollable corridor, with doors which lead to individual
//  topics.
// In the corridor are tappable 3D objects, which present the user with 
//  more info.
class GSMainCorridor : public GSBase
{
public:
  GSMainCorridor();
  virtual void OnActive() override;
  virtual void OnDeactive() override;
  virtual void Update() override;
  virtual void Draw() override;
  virtual void Draw2d() override;

  // Detect swipes, taps etc
  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;

  // Trigger animation, moving to new x-position. 
  // We call this when we swipe left or right, (and there is something in that direction).
  void TriggerCorridorAnim(float desiredX);

  // Conceptually private: button commands
  void EnterClassroomForCurrentTopic();

  // Set current mode: modeId is index into vector, one of the IDs in 
  //  the CorridorMode classes.
  void SetMode(int modeId);

  // Return true if user has unlocked the given topic (int index, not string ID)
  // TODO Move to UserProfile??
  bool IsTopicUnlocked(int topicNum) const;

  // Return true if all Topics in the current Course have been passed,
  //  so player can progress to next level.
  bool AllTopicsPassed() const;

  // Returns true if the current level has all Topics passed.
  bool IsLevelPassed() const;

  // Test each tappable to see if it has been picked
  Tappable* TappablePickTest(const Vec2f& touchCoord);

  void StartDoorAnim();
  void StartEnterLiftAnim();
  void StartExitLiftAnim();

  // Called when we find that user has tapped down and up on a tappable
  void OnTapped(Tappable* tappable);

  // Get the current tappable selected by the user
  Tappable* GetSelectedTappable();

  // Get/set level number: this is shown as different floors, connected
  //  by stairs. The corridors on each floor can look different, and 
  //  could have different numbers of Topics.
  // The Topic group number is the level number - rename it.
  // When we change level:
  //  - Load 3D scene for level number
  //  - Add classrooms for the topics at this level
  void SetLevel(int newLevel);
  int GetLevel() const;

  float GetEnterClassroomAnimTime() const;

protected:
  // Get section we have currently visible in centre of screen 
  PGuiElement GetCurrentCorridorSection();

  // On initialisation, pause anims
  void PauseAnimsOnSection(PGuiElement corridorSection);

  void LoadCorridor();
  void AddRightLiftOrBlankSection(int sectionNum, GuiComposite* addChildren);
  void AddLeftLiftOrBlankSection(GuiComposite* addChildren);
  void AddCorridorSection(int sectionNum, GuiComposite* addChildren);
  void AddOneBlankSection(int sectionNum, GuiComposite* addChildren);
  void AddOneLiftSection(int sectionNum, GuiComposite* addChildren);

  bool LoadTappables();
  void ChangeMode();
  void LoadCourse();

  void UpdateZoom();
  void SetCorridorXPosition();

private:
  RCPtr<GuiDecAnimation> m_posInCorridorAnimator;
  RCPtr<GuiDecAnimation> m_zoomAllAnimator; //?
  RCPtr<GuiDecTranslate> m_posInCorridor;
  float m_lastXPosInCorridor = 0;

  // Each of these sections has a door. We open the door for the chosen topic.
  std::vector<PGuiElement> m_animatableCorridorSections;
  std::vector<PGuiElement> m_animatableLiftSections;

  // Items which respond to being tapped, to show some kind of info.
  // TODO Do they all need a zoom animator then?
  std::vector<RCPtr<Tappable>> m_tappables;

  // Item user has tapped on
  RCPtr<Tappable> m_tapped;

  // The different modes/states we can be in: State pattern
  // The int key is a unique ID for each CorridorMode type.
  std::map<int, RCPtr<CorridorMode>> m_modes;
  RCPtr<CorridorMode> m_currentMode;
  int m_newModeId = -1; // set to a valid index in m_modes when we want to change mode

  int m_levelNum = 1; // 1-based, right?

  float m_zoom = 1.f; // scale factor, zooms in 'camera'
  enum class Zoom
  {
    ZOOM_IN,
    ZOOM_OUT,
    NO_ZOOM
  };
  Zoom m_isZooming;
};

typedef Singleton<GSMainCorridor> TheGSMainCorridor;
}

