#include <iostream>
#include <AmjuRand.h>
#include "AIChasePet.h"
#include "AIEatPet.h"
#include "AIHide.h"
#include "AIIdle.h"
#include "Dino.h"

namespace Amju
{
const char* Dino::NAME = "dino";

Dino::Dino()
{
  CreateSceneNode();

  // Set initial position off screen.
  bool onLeft = RandomInt(2) == 0;
  Vec3f pos(onLeft ? -OFF_SCREEN_X : OFF_SCREEN_X, 0, 0);
  SetPos(pos);

  // Set AIs: we start off hiding.
  auto ai = new AIHide;
  AddAI(ai);
  SetAI(ai);

  AddAI(new AIChasePet);
  AddAI(new AIEatPet);
  AddAI(new AIIdle);
}

const char* Dino::GetTypeName() const
{
  return NAME;
}
}

