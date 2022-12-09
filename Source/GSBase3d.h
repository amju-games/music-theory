// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"

namespace Amju
{
class SceneGraph;

class GSBase3d : public GSBase
{
public:
  virtual void Update() override;
  virtual void Draw() override;
  virtual bool OnKeyEvent(const KeyEvent& ke) override;
  virtual void OnActive() override;

protected:
  SceneGraph* GetSceneGraph();

  // Keyboard camera control, just for debugging
  void DebugCamera(char key);

  // Load scene, using m_sceneFilename
  virtual void Reload3d();

protected:
  std::string m_sceneFilename;
};
}
