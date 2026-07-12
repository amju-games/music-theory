#pragma once

#include "AI.h"

namespace Amju
{
class AIFly : public AI
{
public:
  static const char* NAME;
  const char* GetName() const override;
  void Update() override;
  void OnActivated() override;
};
}
