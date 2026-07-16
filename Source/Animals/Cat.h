#pragma once

#include "Pet.h"

namespace Amju
{
class Cat : public Pet
{
public:
  Cat();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

