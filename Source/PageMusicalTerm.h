// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "MusicalTermQuestion.h"
#include "PageMultiChoice.h"

namespace Amju
{
// * PageMusicalTerm *
// Displays a page which tests the user on musical terms, using multi-choice buttons.
class PageMusicalTerm : public PageMultiChoice
{
public:
  PageMusicalTerm()
  {
    m_guiName = "musical_term";
  }

  static const char* NAME;

  virtual void OnActive() override;
};
}
