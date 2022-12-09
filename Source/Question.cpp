// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Question.h"

namespace Amju
{
std::string Question::GetQuestionString() const
{
  return m_questionString;
}

std::string Question::GetAnswerString() const
{
  return m_answerString;
}

std::string Question::GetExplanationString() const
{
  return m_explanationString;
}

}


