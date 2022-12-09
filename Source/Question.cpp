// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
#include <numeric> // iota()
#include <random>
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

void MultiChoice::SetCorrectAnswer(int correct)
{
  m_correctAnswer = correct;
}

int MultiChoice::GetCorrectAnswer()
{
  return m_correctAnswer;
}

bool MultiChoice::IsAnswerCorrect(int n)
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
	AddTerm(strs[0], strs[1]);
  }
  return true;
}

void MusicalTermsDictionary::AddTerm(const std::string& english, const std::string& foreign)
{
	m_dictionary.push_back(std::make_pair(english, foreign));
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
  // Pick random terms from dictionary.
  int n = m_dictionary->GetNumTerms();
  std::vector<int> nums(n);
  // Fill with ints 0..n
  std::iota(nums.begin(), nums.end(), 0);
  // Randomise
  std::shuffle(nums.begin(), nums.end(), std::mt19937{ std::random_device{}() });

  // Clear answer
  m_answers = MultiChoice();

  // Element zero in shuffled nums is the index of the correct (q, a) pair
  //  in the dictionary. Elements 1 onwards are used to get fake answers.
  std::string ans;
  // TODO Allow switching english/foreign
  m_dictionary->GetTerm(nums[0], &m_musicalTerm, &ans);
  m_answers.AddAnswer(ans);
  m_answers.SetCorrectAnswer(0);

  // TODO set number of fake answers
  int numFakes = 3;
  numFakes = std::min(numFakes, n - 1); // make sure we can't overrun
  for (int i = 1; i <= numFakes; i++)
  {
    std::string q;
    m_dictionary->GetTerm(nums[i], &q, &ans);
    m_answers.AddAnswer(ans);
  }
}

void MusicalTermQuestion::SetDictionary(MusicalTermsDictionary* dictionary)
{
  m_dictionary = dictionary;
}

MultiChoice MultiChoiceQuestion::GetMultiChoiceAnswers()
{
  return m_answers;
}


}


