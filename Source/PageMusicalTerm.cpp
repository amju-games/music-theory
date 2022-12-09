// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include <ResourceManager.h>
#include "PageMusicalTerm.h"

namespace Amju
{
struct ChoiceCommand : public GuiCommand
{
  ChoiceCommand(PageMusicalTerm* page, int button) : m_page(page), m_button(button) {}
  virtual bool Do() override
  {
    m_page->OnChoice(m_button);
    return false;
  }

  int m_button = -1;
  PageMusicalTerm* m_page;
};

void PageMusicalTerm::OnActive()
{
//  Assert(m_question);

  m_question = new MusicalTermQuestion;

  // Get musical terms dictionary (TODO select grade)
  // This is a Resource.
  MusicalTermsDictionary* dic = dynamic_cast<MusicalTermsDictionary*>(
    TheResourceManager::Instance()->GetRes("musical_terms.txt.dictionary"));

  m_question->SetDictionary(dic);
  m_question->MakeQuestion();
  m_answers = m_question->GetMultiChoiceAnswers();

  PageMultiChoice::OnActive();

  GuiText* text = dynamic_cast<GuiText*>(GetElementByName(m_gui, "musical-term-text"));
  text->SetText(m_question->GetMusicalTermText());

  // Set button callbacks
  int numChoices = m_answers.GetNumAnswers(); 
  for (int i = 0; i < numChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    Assert(button);
    button->SetCommand(new ChoiceCommand(this, i));
    button->SetText(m_answers.GetAnswer(i));
  }
  // TODO Hide any extra buttons?
}

void PageMusicalTerm::OnChoice(int c)
{
  std::cout << "Choice " << c << " which is " << (m_answers.IsAnswerCorrect(c) ? "CORRECT!" : "incorrect :(") << "\n";
  
  if (m_answers.IsAnswerCorrect(c))
  {
    // Display tick

    // Happy sound

    // Increase score

    // Add to list of things we know, unless we had hint
  }
  else
  {
    // Display cross

    // Unhappy sound

    // Add to list of things we are weak at

    // Animate fall off screen
  }

  // Next page
}

void PageMusicalTerm::SetMusicalTermQuestion(MusicalTermQuestion* q)
{
  m_question = q;
}

}

