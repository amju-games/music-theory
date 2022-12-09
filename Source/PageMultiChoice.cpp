// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include "GSPages.h"
#include "PageMultiChoice.h"

namespace Amju
{
struct ChoiceCommand : public GuiCommand
{
  ChoiceCommand(PageMultiChoice* page, int button) : m_page(page), m_button(button) {}
  virtual bool Do() override
  {
    m_page->OnChoice(m_button);
    return false;
  }

  PageMultiChoice* m_page;
  int m_button = -1;
};

void PageMultiChoice::OnActive()
{
  Page::OnActive();

  // Set button callbacks
  int numChoices = m_answers.GetNumAnswers();
  int correct = m_answers.GetCorrectAnswer();
  bool seen = m_question->QuestionSeenBefore(m_config);

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
    else if (!seen)
    {
      // Show correct answer for questions we have not seen before.
      button->SetHasFocus(true); // pulsing glow
    }
  }
  // TODO Hide any extra buttons?
}

void PageMultiChoice::OnChoice(int c)
{
  // Disable all buttons
  int numChoices = m_answers.GetNumAnswers();
  for (int i = 0; i < numChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    button->SetIsEnabled(false);
    // Change colour of selected choice
    if (i == c)
    {
      button->SetButtonColour(Colour(0.f, 0.f, 0.f, 1.f));
      button->SetTextColour(Colour(1.f, 1.f, 1.f, 1.f));
    }
  }

  if (m_answers.IsAnswerCorrect(c))
  {
    dynamic_cast<GSPages*>(m_gs)->OnCorrect();
  }
  else
  {
    dynamic_cast<GSPages*>(m_gs)->OnIncorrect();
  }
}

void PageMultiChoice::OnHint()
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

void PageMultiChoice::ShowCorrectAnswer()
{
  int i = m_answers.GetCorrectAnswer();
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
  if (button)
  {
    button->SetHasFocus(true); // pulsing glow
  }
}

}

