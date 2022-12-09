// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuAssert.h>
#include <ROConfig.h>
#include "Consts.h"

namespace Amju
{
Colour GetColour(const char* colourName)
{
  Assert(ROConfig()->Exists(colourName));
  return FromHexString(ROConfig()->GetValue(colourName));
}

}

