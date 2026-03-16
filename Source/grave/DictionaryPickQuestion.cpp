// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include "DictionaryPickQuestion.h"
#include "QuestionPicker.h"
#include "QuestionProgress.h"

namespace Amju
{
void DictionaryPickQuestion::SetDictionary(Dictionary* dictionary)
{
  m_dictionary = dictionary;
}

void DictionaryPickQuestion::MakeQuestion(QuestionProgress& qp)
{
  Assert(m_dictionary);
  const int maxNumQs = m_dictionary->GetNumTerms();
  Assert(maxNumQs == qp.GetMaxQuestions());

  QuestionPicker picker;
  // Pick a number at random from the unused questions in the progress object.
  // This marks this number as used in qp from now on.
  int r = picker.PickCorrect(qp);

  std::string s1, s2;
  m_dictionary->GetTerm(r, &s1, &s2);

  m_questionString = s1;
  m_answerString = s2;
}

bool DictionaryPickQuestion::QuestionSeenBefore(ConfigFile*) const
{
  return false;
}

void DictionaryPickQuestion::SetQuestionSeenBefore(ConfigFile*) const
{
}

}
