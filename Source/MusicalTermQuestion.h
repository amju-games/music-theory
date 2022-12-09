// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "MultiChoiceQuestion.h"

namespace Amju
{
class Dictionary;

class MusicalTermQuestion : public MultiChoiceQuestion
{
public:
  // We get this from a shuffled list, which we get from a file
  std::string GetMusicalTermText();

  virtual void MakeQuestion() override;

  void SetDictionary(Dictionary* dictionary);

  virtual bool QuestionSeenBefore(ConfigFile*) const override;
  virtual void SetQuestionSeenBefore(ConfigFile*) const override;

protected:
  std::string MakeConfigKey() const;

protected:
  std::string m_musicalTerm;

  RCPtr<Dictionary> m_dictionary;
};

}

