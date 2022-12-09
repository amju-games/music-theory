// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Question.h"

namespace Amju
{
std::string Question::GetQuestionString()
{
  Assert(!m_questionStrings.empty());
  return m_questionStrings[0]; // TODO TEMP TEST
}

void Question::AddQuestionString(const std::string& qs)
{
  m_questionStrings.push_back(qs);
}

}


