#pragma once

#include <SceneNode.h>

namespace Amju
{
// Super simple shadow for this game: use as a MIXIN
class SuperSimpleShadowCaster
{
public:
  void SetShadowSceneNode(SceneNode* sn) { m_shadowSceneNode = sn; }
  void SetShadowPosAndSize(const Vec3f& pos, float size);

  // Update shadow scene node, using pos of caster.
  void UpdateShadow(const Vec3f& pos);

  // For ortho view, don't show shadows, saving draw calls.
  // In perspective view (title state) we do want shadows.
  void SetShadowVisible(bool visible);

protected:
  PSceneNode m_shadowSceneNode;
};  
}

