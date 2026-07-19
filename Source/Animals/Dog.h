#pragma once

#include "Pet.h"

namespace Amju
{
class Dog : public Pet
{
public:
  Dog();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

