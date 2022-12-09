// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "PageMultiChoice.h"

namespace Amju
{
class PageIntervalsMultiChoice : public PageMultiChoice
{
public:
  PageIntervalsMultiChoice();
  static const char* NAME;
  virtual void OnActive() override;
};
}


