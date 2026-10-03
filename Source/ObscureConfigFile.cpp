// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "ObscureConfigFile.h"

namespace Amju
{
ObscureConfigFile& GetObscureConfigFile()
{
  static ObscureConfigFile obs;
  return obs;
}
}

