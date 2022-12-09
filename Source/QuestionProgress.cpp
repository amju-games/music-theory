// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
#include <numeric>
#include <vector>
#include <AmjuAssert.h>
#include "QuestionProgress.h"

namespace Amju
{
void QuestionProgress::SetMaxQuestions(int mq)
{
  m_maxNumQuestions = mq;
}

bool QuestionProgress::AllQuestionsUsed() const
{
  int numQs = m_questionsUsed.size();
  Assert(numQs <= m_maxNumQuestions);
  return numQs == m_maxNumQuestions;
}

void QuestionProgress::SetQuestionUsed(int q)
{
  int numQs = m_questionsUsed.size();
  Assert(numQs < m_maxNumQuestions);
  m_questionsUsed.insert(q);
  Assert(m_questionsUsed.size() == numQs + 1);
}

const QuestionSet& QuestionProgress::GetUsedSet() const
{
  return m_questionsUsed;
}

QuestionSet QuestionProgress::GetUnusedSet() const
{
  std::vector<int> nums(m_maxNumQuestions);
  // Fill with ints 0..n
  std::iota(nums.begin(), nums.end(), 0);
  QuestionSet result;

  std::set_symmetric_difference(
    m_questionsUsed.begin(), m_questionsUsed.end(),
    nums.begin(), nums.end(),
    std::inserter(result, result.end())
  );
  return result;
}

int QuestionProgress::GetMaxQuestions() const
{
  return m_maxNumQuestions;
}

int QuestionProgress::GetNumQuestionsUsed() const
{
  return m_questionsUsed.size();
}

}
