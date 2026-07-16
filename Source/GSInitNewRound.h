#pragma once

#include "GSBase3d.h"

namespace Amju
{
// * GSInitNewRound *
// 'One frame' game state that initialises the animals for the 
//  new game round.
// We only activate this state when we start a new round.
class GSInitNewRound : public GSBase3d
{
public:
  void OnActive() override;
};

using TheGSInitNewRound = Singleton<GSInitNewRound>;
}

