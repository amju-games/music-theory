#include <iostream>
#include <AmjuRand.h>
#include "Rabbit.h"

namespace Amju
{
const char* Rabbit::NAME = "rabbit";

Rabbit::Rabbit()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // TODO AIs
}
}

