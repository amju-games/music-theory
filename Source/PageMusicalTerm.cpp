// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiText.h>
#include <ResourceManager.h>
#include "Dictionary.h"
#include "PageMusicalTerm.h"

namespace Amju
{
const char* PageMusicalTerm::NAME = "page-musical-term";

PageMusicalTerm::PageMusicalTerm()
{
  m_guiName = "musical_term";
}

void PageMusicalTerm::OnActive()
{
  MusicalTermQuestion* q = new MusicalTermQuestion;
  m_question = q; 

  // Get musical terms dictionary (TODO select grade)
  // This is a Resource.
  Dictionary* dic = dynamic_cast<Dictionary*>(
    TheResourceManager::Instance()->GetRes("Course/musical_terms.txt.dictionary"));

  q->SetDictionary(dic);
  q->SetCanSwapQAndA(true);
  q->MakeQuestion();
  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = q->QuestionSeenBefore(m_config);
  // Set flag for next time
  q->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();

  // Load gui, set button callbacks
  PageMultiChoice::OnActive();

  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "musical-term-text"));
  text->SetText(q->GetMusicalTermText());
}

}

