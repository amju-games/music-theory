// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "MultiChoiceQuestion.h"

namespace Amju
{
class Dictionary;

class MusicalTermQuestion : public MultiChoiceQuestion
{
public:
  virtual void MakeQuestion(QuestionProgress& qp) override;
  virtual std::string GetAnswerString() const override;

  void SetDictionary(Dictionary* dictionary);

  virtual bool QuestionSeenBefore(ConfigFile*) const override;
  virtual void SetQuestionSeenBefore(ConfigFile*) const override;

protected:
  std::string MakeConfigKey() const;

protected:
  RCPtr<Dictionary> m_dictionary;
};

}

