// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSBase.h"
#include "Question.h"

namespace Amju
{
// * GSMusicalTerms *
// Multi-choice musical term questions
class GSMusicalTerms : public GSBase
{
public:
  GSMusicalTerms();

  // Load appropriate dictionary, split by grades?
  virtual void OnActive() override;

protected:
  RCPtr<MusicalTermsDictionary> m_dictionary;
  MusicalTermQuestion m_question;
};
}
