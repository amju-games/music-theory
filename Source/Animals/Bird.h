#pragma once

#include "PFNpc.h"

namespace Amju
{
class Bird : public PFNpc
{
public:
  Bird();
  static const char* NAME;
  virtual const char* GetTypeName() const override;
};

GameObject* CreateBird();
}

