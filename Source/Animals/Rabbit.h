#pragma once

#include "PFNpc.h"

namespace Amju
{
class Rabbit : public PFNpc
{
public:
  Rabbit();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

