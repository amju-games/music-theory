#pragma once

#include <GameObject.h>
#include "AIController.h"
#include "Md2SceneNodeController.h"

namespace Amju
{
// * Npc *
// Animated (MD2) non-player character with behaviours ('AI').
class Npc : 
  public GameObject, 
  public AIController, 
  public Md2SceneNodeController
{
public:
  void Update() override;

  // Overload to take file name
  bool Load(const std::string& filename);

  bool Load(File*) override;

protected:
  static int CreateId();

protected:
  // Set in ctor.
  // Used to load scene node tree for this game object.
  std::string m_sceneFilename;
};
}

