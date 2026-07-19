#include <AmjuRand.h>
#include "AIEaten.h"
#include "AIFlee.h"
#include "AIIdle.h"
#include "AIJump.h"
#include "AIWander.h"
#include "Pet.h"

namespace Amju
{
Pet::Pet()
{
  // Set initial pos off screen
  float x = OFF_SCREEN_X;
  if (RandomInt(2) == 0) x = -x;
  Vec3f pos(x, 0, 0);
  SetPos(pos);  

  // Add AIs common to all Pets.
  AddAI(new AIEaten);
  AddAI(new AIFlee);
  AddAI(new AIIdle);
  AddAI(new AIJump);
  AddAI(new AIWander);
}
}

