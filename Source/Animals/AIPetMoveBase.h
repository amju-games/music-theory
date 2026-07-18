#pragma once

#include "AI.h"

namespace Amju
{
// Common code for AIs that move a pet.
// Could be obstacle avoidance etc but for this game we just want
//  to not run off screen.
class AIPetMoveBase : public AI
{
protected:
  void StayOnScreen();
};
}

