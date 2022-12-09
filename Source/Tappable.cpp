// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#include <LoadScene.h>
#include "Tappable.h"

namespace Amju
{
bool Tappable::Load(File* f)
{
  // 3D mesh
  m_node = LoadScene(f);
  // Get AABB

  return true;
}

SceneNode* Tappable::GetSceneNode()
{
  return m_node;
}
}
