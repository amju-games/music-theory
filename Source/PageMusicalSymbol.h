// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "PageScoreMCQBase.h"

namespace Amju
{
// * PageMusicalSymbol *
// Show musical symbol to user, who then picks the correct multi-choice button.
class PageMusicalSymbol : public PageScoreMCQBase
{
public:
  PageMusicalSymbol();
  static const char* NAME;
};
}
