// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "Dictionary.h"
#include "Question.h"

namespace Amju
{
// * DictionaryPickQuestion *
// Question picked from dictionary
class DictionaryPickQuestion : public Question
{
public:
  virtual void MakeQuestion() override;
  virtual bool QuestionSeenBefore(ConfigFile*) const override;
  virtual void SetQuestionSeenBefore(ConfigFile*) const override;

  void SetDictionary(Dictionary* dictionary);

protected:
  RCPtr<Dictionary> m_dictionary;
};
}
