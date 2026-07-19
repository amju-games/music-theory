#pragma once

#include "AI.h"

namespace Amju
{
// * AIWait *
// Waits until max time expires before deciding on new AI state.
class AIWait : public AI
{
public:
  static const char* NAME;

  AIWait(float maxTime);
  const char* GetName() const override;
};
}

