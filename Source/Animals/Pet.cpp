#include "AIEaten.h"
#include "AIIdle.h"
#include "AIWander.h"
#include "Pet.h"

namespace Amju
{
Pet::Pet()
{
  // Add AIs common to all Pets.
  AddAI(new AIEaten);
  AddAI(new AIIdle);
  AddAI(new AIWander);
}
}

