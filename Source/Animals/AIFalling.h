#pragma once

#include "AI.h"

namespace Amju
{
class AIFalling : public AI
{
public:
  static const char* NAME;
  AIFalling();
  const char* GetName() const override;
  void Update() override;
};
}

