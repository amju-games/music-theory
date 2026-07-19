#pragma once

#include "AIPetMoveBase.h"

namespace Amju
{
// * AIFlee *
// Decide on direction and how long to go in this direction.
class AIFlee : public AIPetMoveBase
{
public:
  static const char* NAME;
  
  AIFlee();
  void Update() override;
  const char* GetName() const override;
  void OnActivated() override;
  void OnMaxTimeReached() override;
};
}

