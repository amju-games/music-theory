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

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // Each animal type should do this: set up the behaviours
  //  specific to the animal type, and set the initial one.
  auto ai = new AIIdle;
  AddAI(ai);
  SetAI(ai); // The only AI for this type.. right? 

  AddAI(new AIChasePet);
  AddAI(new AIEatPet);
  AddAI(new AIHide);
}

const char* Dino::GetTypeName() const
{
  return NAME;
}
}

