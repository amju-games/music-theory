#pragma once

#include "AI.h"

namespace Amju
{
class AIEaten : public AI
{
public:
  static const char* NAME;
  const char* GetName() const override;
  void OnActivated() override;
  void OnAnimFreeze() override;
};
}

