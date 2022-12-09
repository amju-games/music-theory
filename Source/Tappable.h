// * Amjulib *
// (c) Copyright 2000-2017 Jason Colman

#pragma once

#include <RCPtr.h>

namespace Amju
{
class File;

// * Tappable *
// Tappable items in the main menu corridor - user can tap on these items,
//  and is presented with more info, etc.
class Tappable : public RefCounted
{
public:
  bool Load(File*);
  SceneNode* GetSceneNode();

protected:
  // 3D scene node
  PSceneNode m_node;
};
}