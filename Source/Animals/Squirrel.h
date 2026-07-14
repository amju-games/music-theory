#pragma once

#include "PFNpc.h"

namespace Amju
{
class Squirrel : public PFNpc
{
public:
  Squirrel();
  static const char* NAME;
  const char* GetTypeName() const override { return NAME; }
};
}

