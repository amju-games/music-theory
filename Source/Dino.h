#pragma once

#include "PFNpc.h"

namespace Amju
{
class Dino : public PFNpc
{
public:
  Dino();
  static const char* NAME;
  virtual const char* GetTypeName() const override;
};

GameObject* CreateDino();
}

