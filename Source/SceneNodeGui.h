#pragma once

#include <GuiElement.h>
#include <SceneNode.h>

namespace Amju
{
// * SceneNodeGui *
// GUI which can be part of a 3D SceneGraph
// Game-specfic because we need to know about the camera?
class SceneNodeGui : public SceneNode
{
public:
  static const char* NAME;
  void Update() override;
  void Draw() override;
  bool Load(File*) override;

protected:
  PGuiElement m_gui;
};
}

