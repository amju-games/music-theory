#include <iostream>
#include <AmjuRand.h>
#include "AIWander.h"
#include "Cat.h"

namespace Amju
{
const char* Cat::NAME = "cat";

Cat::Cat()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, 0);
  SetPos(pos);

  // AIs
  SetAI(GetAI("wander"));
}
}

