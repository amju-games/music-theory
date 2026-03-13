// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "Version.h"

#define AMJU_VERSION_MAJOR 0
#define AMJU_VERSION_MINOR 1
#define AMJU_VERSION_PATCH 0

namespace Amju
{
Version& GetVersion()
{
  static Version v;
  return v;
}

std::string Version::ToString() const
{
  return std::to_string(m_major) + "." + 
    std::to_string(m_minor) + "." +
    std::to_string(m_patch);
}
}

