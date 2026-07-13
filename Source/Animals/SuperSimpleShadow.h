#pragma once

#include <SceneNode.h>

namespace Amju
{
// Super simple shadow for this game: use as a MIXIN
struct SuperSimpleShadowCaster
{
  void SetShadowSceneNode(SceneNode* sn) { m_shadowSceneNode = sn; }
  void SetShadowPosAndSize(const Vec3f& pos, float size);

  // Update shadow scene node, using pos of caster.
  void UpdateShadow(const Vec3f& pos);

  PSceneNode m_shadowSceneNode;
};  
}

