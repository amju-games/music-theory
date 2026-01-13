#include "Suppress.h"

static unsigned int flags = 0;

void SetSuppressFlags(unsigned int suppressFlags)
{
  flags |= suppressFlags;
}

unsigned int GetSuppressFlags()
{
  return flags;
}


