// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
class SceneGraph;

// * GSBase3d *
// Base class for Game States which display a 3D view.
class GSBase3d : public GSBase
{
public:
  void Update() override;
  void Draw() override;

  // Key input: this is for debug camera
  bool OnKeyEvent(const KeyEvent& ke) override;

  void OnActive() override;

  // As 3d scene can be resource-heavy, trash resources here.
  void OnDeactive() override;

  bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  bool OnCursorEvent(const CursorEvent&) override;

protected:
  SceneGraph* GetSceneGraph();

  // Keyboard camera control, just for debugging
  void DebugCamera(char key);

  // Load scene, using m_sceneFilename
  virtual void Reload3d();

protected:
  // Filename of scene this game state loads and displays.
  // Set this in derived class ctors.
  std::string m_sceneFilename;
};
}
