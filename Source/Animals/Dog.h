#pragma once

#include "PFNpc.h"

namespace Amju
{
class Dog : public PFNpc
{
public:
  Dog();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

