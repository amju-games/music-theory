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
};
}

