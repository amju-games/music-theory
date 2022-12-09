// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <ResourceManager.h>
#include "GSPages.h"
#include "PageMusicalTerm.h"
#include "PrintGui.h"

namespace Amju
{
const char* PageMusicalTerm::NAME = "page-musical-term";

void PageMusicalTerm::OnActive()
{
  m_question = new MusicalTermQuestion;

  // Get musical terms dictionary (TODO select grade)
  // This is a Resource.
  MusicalTermsDictionary* dic = dynamic_cast<MusicalTermsDictionary*>(
    TheResourceManager::Instance()->GetRes("Course/musical_terms.txt.dictionary"));

  MusicalTermQuestion* q = dynamic_cast<MusicalTermQuestion*>(m_question.GetPtr());
  q->SetDictionary(dic);
  q->MakeQuestion();
  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = m_question->QuestionSeenBefore(m_config);
  // Set flag for next time
  m_question->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();

  PageMultiChoice::OnActive();

  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "musical-term-text"));
  text->SetText(q->GetMusicalTermText());
}

void PageMusicalTerm::SetMusicalTermQuestion(MusicalTermQuestion* q)
{
  m_question = q;
}

}

