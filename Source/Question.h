// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <string>
#include <vector>
#include <ConfigFile.h>
#include <RCPtr.h>
#include <StringUtils.h>

namespace Amju
{
class QuestionProgress;

// Base class for questions, the logical, non-presentation representation 
//  of questions we ask the user. So should be completely testable... right?
class Question : public RefCounted
{
public:
  virtual ~Question() = default;

  virtual std::string GetQuestionString() const;
  virtual std::string GetAnswerString() const;
  virtual std::string GetExplanationString() const;

  virtual void MakeQuestion(QuestionProgress& qp) = 0;

  // Get/set flag in config file, so we know if a question has been seen before.
  // (We give more info if not seen before, etc.)
  virtual bool QuestionSeenBefore(ConfigFile*) const = 0;
  virtual void SetQuestionSeenBefore(ConfigFile*) const = 0;

protected:
  std::string m_questionString;
  std::string m_answerString;
  std::string m_explanationString;
};
}

