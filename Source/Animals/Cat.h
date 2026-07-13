#pragma once

#include "PFNpc.h"

namespace Amju
{
class Cat : public PFNpc
{
public:
  Cat();
  static const char* NAME;
  virtual const char* GetTypeName() const override;
};
}

