#include <iostream>
#include <AmjuRand.h>
#include "AIFly.h"
#include "Bird.h"

namespace Amju
{
const char* Bird::NAME = "bird";

GameObject* CreateBird() { return new Bird; }

Bird::Bird()
{
  // Bummer: we can't do this in the base class ctor, we won't know
  //  our full type yet. :(
  CreateSceneNode(); 

  // Type-specific initial position
  Vec3f pos(Rnd(-10, 10) * 20, 100, Rnd(-10, 10) * 20);
  SetPos(pos);

  // Each animal type should do this: set up the behaviours
  //  specific to the animal type, and set the initial one.
  auto ai = new AIFly;
  AddAI(ai);
  SetAI(ai); // The only AI for this type.. right? 
}

const char* Bird::GetTypeName() const
{
  return NAME;
}
}

