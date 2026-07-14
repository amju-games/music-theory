#include <iostream>
#include <AmjuRand.h>
#include "Dog.h"

namespace Amju
{
const char* Dog::NAME = "dog";

Dog::Dog()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // TODO AIs
}
}

