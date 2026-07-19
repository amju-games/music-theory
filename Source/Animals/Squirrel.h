#pragma once

#include "Pet.h"

namespace Amju
{
class Squirrel : public Pet
{
public:
  Squirrel();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

