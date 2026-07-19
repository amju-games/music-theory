#pragma once

#include "Pet.h"

namespace Amju
{
class Rabbit : public Pet
{
public:
  Rabbit();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

