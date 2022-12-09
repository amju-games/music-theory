// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
#include <numeric> // iota()
#include <random>
#include <ConfigFile.h>
#include <File.h>
#include <StringUtils.h>
#include "Dictionary.h"
#include "MusicalTermQuestion.h"

namespace Amju
{
std::string MusicalTermQuestion::GetMusicalTermText()
{
  return m_musicalTerm;
}

void MusicalTermQuestion::MakeQuestion()
{
  // Pick random terms from dictionary.
  const int maxNumQs = m_dictionary->GetNumTerms();
  std::vector<int> nums(maxNumQs);
  // Fill with ints 0..n
  std::iota(nums.begin(), nums.end(), 0);
  // Randomise
  std::shuffle(nums.begin(), nums.end(), std::mt19937{ std::random_device{}() });

  // Clear answer
  m_answers = MultiChoice();

  // Allow switching english/foreign: decide whether to switch
  if (m_canSwapQAndA)
  {
    m_qAndASwitched = (rand() & 1) != 0;
  }

  // TODO set number of fake answers
  int numAnswers = 4;
  numAnswers = std::min(numAnswers, maxNumQs); // make sure we can't overrun
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

    // If this answer already exists, move to next. This is to deal with multiple different 
    //  questions which have the same answer.
    if (m_answers.IsAnAnswer(ans))
    {
      // We need one more 
      numAnswers++;
      numAnswers = std::min(numAnswers, maxNumQs); // make sure we can't overrun
      continue;
    }

    m_answers.AddAnswer(ans);
    if (i == correct)
    {
      m_musicalTerm = q;
      m_questionStrings = { q };
    }
  }
}

void MusicalTermQuestion::SetDictionary(Dictionary* dictionary)
{
  m_dictionary = dictionary;
}

std::string MusicalTermQuestion::MakeConfigKey() const
{
  std::string key = m_musicalTerm;
  if (m_qAndASwitched)
  {
    key = m_answers.GetAnswer(m_answers.GetCorrectAnswer());
  }

  key = Replace(key, " ", "_");
  return key;
}

bool MusicalTermQuestion::QuestionSeenBefore(ConfigFile* cf) const
{
  std::string key = MakeConfigKey() + "_SEEN";
  return cf->Exists(key);
}

void MusicalTermQuestion::SetQuestionSeenBefore(ConfigFile* cf) const
{
  std::string key = MakeConfigKey() + "_SEEN";
  cf->SetInt(key, 1);
}

}

