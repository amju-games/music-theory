// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <vector>
#include <RCPtr.h>

namespace Amju
{
// Set of indices into a Dictionary, used to represent questions which
//  hae been used, or not used.
// No duplicates, but we use vector so we can pick an element at random,
//  (and for general cache-friendliness).
using QuestionSet = std::vector<int>;

// * QuestionProgress *
// Stores progress through a dictionary of questions. This is used to
//  make sure we ask every question once, and we know when we have 
//  asked all the Qs.
class QuestionProgress : public RefCounted
{
public:
  void SetMaxQuestions(int mq);
  bool AllQuestionsUsed() const;
  void SetQuestionUsed(int q);
 
  // Get set of used questions, so we don't reuse a question already
  //  asked.
  const QuestionSet& GetUsedSet() const;

  // Get the complement of the used set, (we know how many Qs there are).
  QuestionSet GetUnusedSet() const;
 
  // These functions are useful for testing, but are not required otherwise?
  int GetMaxQuestions() const;
  int GetNumQuestionsUsed() const;

private:
  // Set of question numbers, zero based.
  QuestionSet m_questionsUsed;
  int m_maxNumQuestions = 0; 
};
}

