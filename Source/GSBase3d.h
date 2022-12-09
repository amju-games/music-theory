// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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
  virtual void Update() override;
  virtual void Draw() override;

  // Key input: this is for debug camera
  virtual bool OnKeyEvent(const KeyEvent& ke) override;

  virtual void OnActive() override;

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
