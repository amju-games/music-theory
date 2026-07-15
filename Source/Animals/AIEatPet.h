#pragma once

#include "AI.h"

namespace Amju
{
class Pet;

class AIEatPet : public AI
{
public:
  static const char* NAME;

  AIEatPet();
  const char* GetName() const override;
  void Update() override;
  void OnActivated() override;
  void OnAnimFinished() override;
};
}

