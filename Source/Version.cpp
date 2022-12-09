// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Version.h"

#define AMJU_VERSION_MAJOR 1
#define AMJU_VERSION_MINOR 0

namespace Amju
{
std::string GetVersionStr()
{
  return std::to_string(AMJU_VERSION_MAJOR) + "." + std::to_string(AMJU_VERSION_MINOR);
}
}

