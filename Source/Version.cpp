// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "Version.h"

#define AMJU_VERSION_MAJOR 0
#define AMJU_VERSION_MINOR 1
#define AMJU_VERSION_PATCH 0

namespace Amju
{
std::string GetVersionStr()
{
  return std::to_string(AMJU_VERSION_MAJOR) + "." + 
    std::to_string(AMJU_VERSION_MINOR) + "." +
    std::to_string(AMJU_VERSION_PATCH);
}
}

