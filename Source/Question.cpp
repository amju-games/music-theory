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

bool Question::Load(File* f)
{
  int numQs = 0;
  if (!f->GetInteger(&numQs))
  {
    f->ReportError("Expected num question strings");
    return false;
  }

  for (int i = 0; i < numQs; i++)
  {
    std::string s;
    if (!f->GetDataLine(&s))
    {
      f->ReportError("Expected question string " + ToString(i + 1) + " of " + ToString(numQs));
      return false;
    }
    m_questionStrings.push_back(s);
  }

  return true;
}

int MultiChoice::GetCorrectAnswer()
{
  return m_correctAnswer;
}

bool MultiChoice::IsAnswerCorrect(int n)
{
  return n == m_correctAnswer;
}


bool MusicalTermsDictionary::Load(const std::string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    return false;
  }
  std::string line;
  while (f.GetDataLine(&line))
  {
    // Split English/foreign strings 
    Strings strs = Split(line, '=');
    if (strs.size() != 2)
    {
      // Not sure whether to make this an error
      f.ReportError("Finished loading dictionary: " + line);
      break;
    }
    m_dictionary.push_back(std::make_pair(strs[0], strs[1]));
  }
  return true;
}

int MusicalTermsDictionary::GetNumTerms() const
{
  return m_dictionary.size();
}

void MusicalTermsDictionary::GetTerm(int i, std::string* english, std::string* foreign) const
{
  const auto& p = m_dictionary[i];
  *english = p.first;
  *foreign = p.second;
}

std::string MusicalTermQuestion::GetMusicalTermText()
{
  return m_musicalTerm;
}

void MusicalTermQuestion::MakeQuestion()
{
  // Pick random term from dictionary.

  // Pick more random terms for MCQ false answers.
}

void MusicalTermQuestion::SetDictionary(MusicalTermsDictionary* dictionary)
{
  m_dictionary = dictionary;
}

MultiChoice MusicalTermQuestion::GetMultiChoiceAnswers()
{
  return m_answers;
}


}


