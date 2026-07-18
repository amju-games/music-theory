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

  static const char* BLOODY_TEX_1;
  static const char* BLOODY_TEX_2;
  static const char* NO_BLOOD_TEX_1;
  static const char* NO_BLOOD_TEX_2;
};
}

