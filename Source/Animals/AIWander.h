#pragma once

#include "AI.h"

namespace Amju
{
// * AIWander *
// Decide on direction and how long to go in this direction.
class AIWander : public AI
{
public:
  static const char* NAME;
  
  AIWander();
  void Update() override;
  const char* GetName() const override;
  void OnActivated() override;
  float GetRank() override; // YOU CAN'T JUST CALCULATE RANK IN ONACTIVATED
};
}

