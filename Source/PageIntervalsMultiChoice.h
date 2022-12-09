// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "PageScoreMCQBase.h"

namespace Amju
{
// * PageIntervalsMultiChoice *
// Show/play interval to user, who then picks the correct multi-choice button.
// Hint should count the degrees, so quite different to other score/MCQ pages.
class PageIntervalsMultiChoice : public PageScoreMCQBase
{
public:
  PageIntervalsMultiChoice();
  static const char* NAME;
};
}


