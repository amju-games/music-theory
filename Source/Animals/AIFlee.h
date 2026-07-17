#pragma once

#include "AI.h"

namespace Amju
{
// * AIFlee *
// Decide on direction and how long to go in this direction.
class AIFlee : public AI
{
public:
  static const char* NAME;
  
  AIFlee();
  void Update() override;
  const char* GetName() const override;
  void OnActivated() override;
};
}

