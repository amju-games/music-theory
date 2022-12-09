// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

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

  virtual void OnActive() override;

  void OnChoice(int c);

  void SetMusicalTermQuestion(MusicalTermQuestion* q);

protected:
  RCPtr<MusicalTermQuestion> m_question;
  MultiChoice m_answers;
};
}
