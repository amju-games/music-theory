// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
#include <numeric> // iota()
#include <random>
#include <ConfigFile.h>
#include <File.h>
#include <StringUtils.h>
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

/*
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
*/

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

  // Allow switching english/foreign: decide whether to switch
  m_qAndASwitched = (rand() & 1) != 0;

  // TODO set number of fake answers
  int numAnswers = 4;
  numAnswers = std::min(numAnswers, n - 1); // make sure we can't overrun
  int correct = rand() % numAnswers;
  m_answers.SetCorrectAnswer(correct);
  for (int i = 0; i < numAnswers; i++)
  {
    std::string ans;
    std::string q;

    m_dictionary->GetTerm(nums[i], &q, &ans);

    if (m_qAndASwitched)
    {
      std::swap(q, ans);
    }

    m_answers.AddAnswer(ans);
    if (i == correct)
    {
      m_musicalTerm = q;
    }
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

bool MusicalTermQuestion::QuestionSeenBefore(ConfigFile* cf) const
{
  // TODO Factor out
  std::string key = m_musicalTerm;
  if (m_qAndASwitched)
  {
    key = m_answers.GetAnswer(m_answers.GetCorrectAnswer());
  }

  key = Replace(key, " ", "_");
  key += "_SEEN";

  return cf->Exists(key);
}

void MusicalTermQuestion::SetQuestionSeenBefore(ConfigFile* cf) const
{
  std::string key = m_musicalTerm;
  if (m_qAndASwitched)
  {
    key = m_answers.GetAnswer(m_answers.GetCorrectAnswer());
  }

  key = Replace(key, " ", "_");
  key += "_SEEN";
  cf->SetInt(key, 1);
}

}


