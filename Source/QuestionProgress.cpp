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
  // Should not already be a member of m_questionsUsed
  Assert(std::find(m_questionsUsed.begin(), m_questionsUsed.end(), q) == 
    m_questionsUsed.end());

  int numQs = m_questionsUsed.size();
  Assert(numQs < m_maxNumQuestions);
  m_questionsUsed.push_back(q);

  // Sort - required for set_symmetric_difference below, and for testing,
  //  we want to compare vectors pretending to be sets.
  std::sort(m_questionsUsed.begin(), m_questionsUsed.end());

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

  // m_questionsUsed is sorted, so set_symmetric_difference is ok here.
  std::set_symmetric_difference(
    m_questionsUsed.begin(), m_questionsUsed.end(),
    nums.begin(), nums.end(),
    std::back_inserter(result)
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
