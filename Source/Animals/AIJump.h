#pragma once

#include "AIPetMoveBase.h"

namespace Amju
{
// * AIJump *
// Happy jumps
class AIJump : public AIPetMoveBase
{
public:
  static const char* NAME;
  const char* GetName() const override;
  
  AIJump();
  void Update() override;
  void OnActivated() override;
};
}

