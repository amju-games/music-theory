#pragma once

#include "Md2SceneNode.h" // TODO Promote to amjulib

namespace Amju
{
// * Md2SceneNodeController *
// MIXIN class.
// Animated (MD2) non-player character.
class Md2SceneNodeController 
{
public:
  virtual ~Md2SceneNodeController() = default;
 
  // Update turn angle and pos on scene node.
  void UpdateMd2(const Vec3f& pos);

  void SetAnim(const std::string& animName);

  // Get/set the scene node 
  SceneNode* GetSceneNode() { return m_sceneNode; }
  void SetSceneNode(Md2SceneNode* sceneNode) { m_sceneNode = sceneNode; }

  // Direction: this is desired direction (around y-axis), to which
  //  we turn over time, going the shortest way.
  void SetDir(float degs) { m_dir = degs; }
  float GetDir() const { return m_dir; }

protected:
  // The MD2 scene node, not the whole tree for the character necessarily,
  //  as this could include shadow and other stuff.
  RCPtr<Md2SceneNode> m_sceneNode;

  float m_dir = 0; // Direction of movement - DEGREES
  // Direction we currently face -- we will reduce the difference
  //  between m_dirCurrent and m_dir until we face the direction of movement.
  float m_dirCurrent = 0;

  // MD2 anim index
  int m_anim;
};
}

