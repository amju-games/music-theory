#include <iostream>
#include <AmjuRand.h>
#include "Cat.h"

namespace Amju
{
const char* Cat::NAME = "cat";

Cat::Cat()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // TODO AIs
}

const char* Cat::GetTypeName() const
{
  return NAME;
}
}

