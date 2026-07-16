#include "AIEaten.h"
#include "AIIdle.h"
#include "AIWander.h"
#include "Pet.h"

namespace Amju
{
Pet::Pet()
{
  // Add AIs common to all Npcs.
  // Better if there was a base class just for edible animals
  //  but this works ok. Maybe a big dino eats smaller dinos.
  AddAI(new AIEaten);
  AddAI(new AIIdle);
  AddAI(new AIWander);
}
}

