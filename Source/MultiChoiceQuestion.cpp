// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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

MultiChoice MultiChoiceQuestion::GetMultiChoiceAnswers()
{
  return m_answers;
}

bool MultiChoice::IsAnAnswer(const std::string& a) const
{
  return std::find(m_answers.begin(), m_answers.end(), a) != m_answers.end();
}

void MultiChoiceQuestion::SetCanSwapQAndA(bool canSwap)
{
  m_canSwapQAndA = canSwap;
}

}


