// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Version.h"

namespace Amju
{
std::string GetVersionStr()
{
  char buf[16];
  sprintf(buf, "%d.%d", AMJU_VERSION_MAJOR, AMJU_VERSION_MINOR);
  return buf;
}
}

