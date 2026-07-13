#include "SuperSimpleShadow.h"

namespace Amju
{
void SuperSimpleShadowCaster::SetShadowPosAndSize(const Vec3f& pos, float size)
{
  Assert(m_shadowSceneNode);

  Matrix mtx;
  mtx.Scale(size, 1.f, size);
  mtx.TranslateKeepRotation(pos);
 
  m_shadowSceneNode->SetLocalTransform(mtx);
}

void SuperSimpleShadowCaster::UpdateShadow(const Vec3f& pos)
{
  // Set size depending on height from ground.
  // Max size is 1, min size 0
  const float maxH = 100.f; // TODO member var
  float shadowSize = (maxH - pos.y) / maxH;
  shadowSize = std::clamp(shadowSize, 0.f, 1.f);
  SetShadowPosAndSize({pos.x, 0, pos.z}, shadowSize);
}
}

