#include <iostream>
#include <AmjuRand.h>
#include "Squirrel.h"

namespace Amju
{
const char* Squirrel::NAME = "squirrel";

Squirrel::Squirrel()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // TODO AIs
}
}

