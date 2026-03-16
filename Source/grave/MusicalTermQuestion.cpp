// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <numeric> // iota()
#include <random>
#include <ConfigFile.h>
#include <File.h>
#include <StringUtils.h>
#include "Dictionary.h"
#include "MusicalTermQuestion.h"
#include "QuestionPicker.h"
#include "QuestionProgress.h"

namespace Amju
{
void MusicalTermQuestion::MakeQuestion(QuestionProgress& qp)
{
  // Pick random terms from dictionary.
  const int maxNumQs = m_dictionary->GetNumTerms();
  Assert(maxNumQs == qp.GetMaxQuestions());

  QuestionPicker picker;
  // Pick a number at random from the unused questions in the progress object.
  // This marks this number as used in qp from now on.
  int correct = picker.PickCorrect(qp);
  // Pick fake, wrong answers ('distractors'). We need the dictionary to 
  //  make sure all the answers are unique.
  const int MAX_NUM_FAKES = 3; // TODO should be configurable
  std::vector<int> fakes = picker.GetNFakes(MAX_NUM_FAKES, correct, *m_dictionary);
  // TODO currently not using m_canSwapQAndA

  // Mix up correct and fake answers. 
  fakes.insert(fakes.begin(), correct); // Correct answer is at index 0...
  // Swap element 0 with some other position
  int correctIndex = rand() % fakes.size(); // TODO don't use rand()
  std::swap(fakes[0], fakes[correctIndex]); // Now the correct answer is mixed into the fakes

  // Get the question text ('stem'), actual answer, and explanation for the correct answer. 
  std::string ans;
  m_dictionary->GetTerm(correct, &m_questionString, &ans, &m_explanationString);
  m_answers.SetCorrectAnswer(correctIndex);

  // Get the strings for the correct and fake answers.
  int numFakes = fakes.size();
  for (int i = 0; i < numFakes; i++)
  {
    std::string questionStr;
    m_dictionary->GetTerm(fakes[i], &questionStr, &ans);
    m_answers.AddAnswer(ans);
  }
}

void MusicalTermQuestion::SetDictionary(Dictionary* dictionary)
{
  m_dictionary = dictionary;
}

std::string MusicalTermQuestion::MakeConfigKey() const
{
  std::string key = m_questionString;
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

std::string MusicalTermQuestion::GetAnswerString() const
{
  int correct = m_answers.GetCorrectAnswer();
  return m_answers.GetAnswer(correct);
}
}

