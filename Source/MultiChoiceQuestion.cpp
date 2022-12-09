// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "MultiChoiceQuestion.h"

namespace Amju
{
const std::string& MultiChoice::GetAnswer(int n) const
{
  return m_answers[n];
}

void MultiChoice::SetCorrectAnswer(int correct)
{
  m_correctAnswer = correct;
}

int MultiChoice::GetCorrectAnswer() const
{
  return m_correctAnswer;
}

bool MultiChoice::IsAnswerCorrect(int n) const
{
  return n == m_correctAnswer;
}

void MultiChoice::AddAnswer(const std::string& answer)
{
  m_answers.push_back(answer);
}

int MultiChoice::GetNumAnswers() const
{
  return static_cast<int>(m_answers.size());
}

}


