// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Dictionary.h"
#include "MusicalTermQuestion.h"
#include "PageIntervalsMultiChoice.h"

namespace Amju
{
const char* PageIntervalsMultiChoice::NAME = "page-intervals-multi-choice";

PageIntervalsMultiChoice::PageIntervalsMultiChoice()
{
  m_guiName = "interval";
}

void PageIntervalsMultiChoice::OnActive()
{
  MusicalTermQuestion* q = new MusicalTermQuestion;
  m_question = q;

  // Get musical terms dictionary (TODO select grade)
  // This is a Resource.
  Dictionary* dic = dynamic_cast<Dictionary*>(
    TheResourceManager::Instance()->GetRes("Course/intervals.txt.dictionary"));

  q->SetDictionary(dic);
  q->MakeQuestion();
  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = q->QuestionSeenBefore(m_config);
  // Set flag for next time
  q->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();

  // Load gui, set button callbacks
  PageMultiChoice::OnActive();
}

}


