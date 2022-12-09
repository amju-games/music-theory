// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSShowLineDrawing *
// Test game state: shows line drawing
class GSShowLineDrawing : public GSBase
{
public:
  GSShowLineDrawing();
};

typedef Singleton<GSShowLineDrawing> TheGSShowLineDrawing;
}

