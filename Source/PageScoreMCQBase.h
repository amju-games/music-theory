// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "PageMultiChoice.h"

namespace Amju
{
// * PageScoreMCQBase *
// Base class for pages which have a musical score/symbols, and multi-choice
//  question buttons. The questions are loaded from a dictionary file, and used
//  to populate the music score and buttons. So the main difference between 
//  subclasses could be the dictionary used.
class PageScoreMCQBase : public PageMultiChoice
{
public:
  virtual void OnActive() override;
};
}

