#pragma once

#include "AI.h"

namespace Amju
{
// Dino runs off screen to hide.
class AIHide : public AI
{
public:
  static const char* NAME;
  const char* GetName() const override;
  void OnActivated() override;
  void Update() override;
};
}

