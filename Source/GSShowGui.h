// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSShowGui *
// Test game state: shows line drawing
class GSShowGui : public GSBase
{
public:
  GSShowGui();
};

typedef Singleton<GSShowGui> TheGSShowGui;
}

