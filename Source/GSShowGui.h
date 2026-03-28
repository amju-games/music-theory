// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
// * GSShowGui *
// Test game state: shows GUI specified as final command line param.
// This is the start state if you specify --gui on the command line.
// E.g. ./mygame --gui mygui.txt
class GSShowGui : public GSBase
{
public:
  GSShowGui();
};

typedef Singleton<GSShowGui> TheGSShowGui;
}

