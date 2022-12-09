// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "DictionaryPickQuestion.h"

namespace Amju
{
void DictionaryPickQuestion::SetDictionary(Dictionary* dictionary)
{
  m_dictionary = dictionary;
}

void DictionaryPickQuestion::MakeQuestion()
{
  Assert(m_dictionary);
  m_questionStrings.clear();

  std::string s1, s2;
  int num = m_dictionary->GetNumTerms();
  int r = rand() % num;
  m_dictionary->GetTerm(r, &s1, &s2);

  m_questionStrings.push_back(s1); // TODO
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
