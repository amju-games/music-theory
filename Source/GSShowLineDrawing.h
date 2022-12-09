// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GSShowLineDrawing : public GSBase
{
public:
  GSShowLineDrawing();
};

typedef Singleton<GSShowLineDrawing> TheGSShowLineDrawing;
}

