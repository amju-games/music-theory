#include <iostream>
#include <AmjuRand.h>
//#include "AIFly.h"
#include "Dino.h"

namespace Amju
{
const char* Dino::NAME = "dino";

GameObject* CreateDino() { return new Dino; }

Dino::Dino()
{
  CreateSceneNode();

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 0, Rnd(-10, 10) * 20);
  SetPos(pos);

  // Each animal type should do this: set up the behaviours
  //  specific to the animal type, and set the initial one.
//  auto ai = new AIFly;
//  AddAI(ai);
//  SetAI(ai); // The only AI for this type.. right? 
}

const char* Dino::GetTypeName() const
{
  return NAME;
}
}

