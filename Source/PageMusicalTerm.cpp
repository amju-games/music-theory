// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <ResourceManager.h>
#include "GSPages.h"
#include "PageMusicalTerm.h"

namespace Amju
{
// TODO put somewhere else
// Print GUI tree, useful for finding gui file errors
void PrintGui(GuiElement* gui, int depth)
{
  std::string spaces(2 * depth, ' ');
  //std::string type = typeid(gui).name();
  std::cout << spaces << gui->GetName(); // << " (" << type << ")";
  GuiComposite* comp = dynamic_cast<GuiComposite*>(gui);
  if (comp)
  {
    int n = comp->GetNumChildren();

    std::cout << " " << n << (n > 1 ? " children" : " child") << ":\n";

    for (int i = 0; i < n; i++)
    {
      PrintGui(comp->GetChild(i), depth + 1);
    }
  }
  else
  {
    std::cout << "\n";
  }
}

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
    TheResourceManager::Instance()->GetRes("Course/musical_terms.txt.dictionary"));

  m_question->SetDictionary(dic);
  m_question->MakeQuestion();
  m_answers = m_question->GetMultiChoiceAnswers();

  PageMultiChoice::OnActive();

//  PrintGui(m_gui, 0);

  GuiText* text = dynamic_cast<GuiText*>(GetElementByName(m_gui, "musical-term-text"));
  text->SetText(m_question->GetMusicalTermText());

  // Set button callbacks
  int numChoices = m_answers.GetNumAnswers(); 
  int correct = m_answers.GetCorrectAnswer();

  for (int i = 0; i < numChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    Assert(button);
    button->SetCommand(new ChoiceCommand(this, i));
    button->SetText(m_answers.GetAnswer(i));

    // Store buttons we can remove to give a hint
    if (i != correct)
    {
      m_canRemoveForHint.push_back(i);
    }
//    OnHint(); // if Q not seen before
    // Set hint for questions we have not seen before.
    // Also do this if user taps Hint button, which should affect score and add this Q
    //  to the list that needs extra reinforcement.
    //if (m_answers.IsAnswerCorrect(i))
    //{
    //  button->SetHasFocus(true); // pulsing glow
    //}
  }
  // TODO Hide any extra buttons?
}

void PageMusicalTerm::OnHint()
{
  // Fade out an incorrect answer. Shunt the buttons underneath up to fill the gap.
  if (m_canRemoveForHint.size() < 1)
  {
    return;
  }

  std::random_shuffle(m_canRemoveForHint.begin(), m_canRemoveForHint.end());
  int n = m_canRemoveForHint.back(); // the button to remove
  m_canRemoveForHint.pop_back(); // so we can't remove it again

  std::string fadeAnimName = "fade-button-" + ToString(n);
  GuiDecAnimation* fadeAnim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, fadeAnimName));
  // Change value of this anim from zero to one, enabling desendant animations
  fadeAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

  // Disable the button
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(n)));
  button->SetIsEnabled(false);
}

void PageMusicalTerm::ShowCorrectAnswer()
{
  int i = m_answers.GetCorrectAnswer();
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
  if (button)
  {
    button->SetHasFocus(true); // pulsing glow
  }
}

void PageMusicalTerm::OnChoice(int c)
{
  std::cout << "Choice " << c << " which is " << (m_answers.IsAnswerCorrect(c) ? "CORRECT!" : "incorrect :(") << "\n";
  
  if (m_answers.IsAnswerCorrect(c))
  {
    dynamic_cast<GSPages*>(m_gs)->OnCorrect();


    // Increase score

    // Add to list of things we know, unless we had hint
  }
  else
  {
    dynamic_cast<GSPages*>(m_gs)->OnIncorrect();

    // Display cross


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

