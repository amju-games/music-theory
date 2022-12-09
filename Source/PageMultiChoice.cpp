// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <Localise.h>
#include <ReportError.h>
#include "Consts.h"
#include "Dictionary.h"
#include "GSPages.h"
#include "GuiMusicScore.h"
#include "LurkMsg.h"
#include "MusicalTermQuestion.h"
#include "PageMultiChoice.h"

namespace Amju
{
// Called when we dismiss the explanation for an incorrect answer
static void OnDismissedExplanation(GuiElement*)
{
  Page::SendNextPageMessage();
}

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
  SetUpQuestion(); // decide on Q to ask, and real + false answers

  Page::OnActive();

  SetUpButtons();

  SetUpQuestionUI();

  GuiElement* correct = GetElementByName(m_gui, "animate-correct");
  correct->SetVisible(false);
}

void PageMultiChoice::SetUpButtons()
{
  // Set button callbacks
  int numChoices = m_answers.GetNumAnswers();
  int correct = m_answers.GetCorrectAnswer();
  bool seen = m_question->QuestionSeenBefore(m_config);

  for (int i = 0; i < numChoices; i++)
  {
    std::string istr = ToString(i);
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + istr));
    Assert(button);
    button->SetCommand(new ChoiceCommand(this, i));

    // TODO TEMP TEST
    const std::string& answerStr = m_answers.GetAnswer(i);
    if (m_aType == AnswerType::ATYPE_SCORE)
    {
      GuiMusicScore* ms = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score-" + istr));
      Assert(ms);
      ms->AddMultipleGlyphsFromString(answerStr);
    }
    else if (m_aType == AnswerType::ATYPE_TEXT)
    {
      IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "text-choice-" + istr));
      Assert(text);
      text->SetText(answerStr);
    }

    // Store buttons we can remove to give a hint
    if (i != correct)
    {
      m_canRemoveForHint.push_back(i);
    }
    else if ((!seen) || (numChoices == 1))
    {
      // Show correct answer for questions we have not seen before.
      button->SetHasFocus(true); // pulsing glow
    }
  }

  // Hide any extra buttons
  int maxNumChoices = 4; // ? or just keep going until we fail to find any more buttons?
  for (int i = numChoices; i < maxNumChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    if (!button)
    {
      break;
    }
    button->SetVisible(false);
  }
}

void PageMultiChoice::OnChoice(int c)
{
  // Disable all buttons. Get position of choice.
  Vec2f choicePos;
  int numChoices = m_answers.GetNumAnswers();
  for (int i = 0; i < numChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    button->SetIsEnabled(false);

    // TODO This has no effect because buttons are transparent regions?!
//    // Change colour of selected choice
//    if (i == c)
//    {
//      button->SetButtonColour(Colour(0.f, 0.f, 0.f, 1.f));
//      button->SetTextColour(Colour(1.f, 1.f, 1.f, 1.f));
//      choicePos = button->GetCombinedPos();
//    }
  }

  // Position offset for tick/cross
  const Vec2f OFFSET(-0.03f, -0.15f);

  if (m_answers.IsAnswerCorrect(c))
  {
    dynamic_cast<GSPages*>(m_gs)->OnCorrect(choicePos + OFFSET);
  }
  else
  {
    dynamic_cast<GSPages*>(m_gs)->OnIncorrect(choicePos + OFFSET);
  }
}

bool PageMultiChoice::CanGetHint()
{
  return !m_canRemoveForHint.empty();
}

void PageMultiChoice::OnHint()
{
  // Fade out an incorrect answer.
  if (m_canRemoveForHint.size() < 1)
  {
    return;
  }

  std::random_shuffle(m_canRemoveForHint.begin(), m_canRemoveForHint.end());
  int n = m_canRemoveForHint.back(); // the button to remove

  HideChoiceButton(n);

  if (m_canRemoveForHint.empty())
  {
    // Pulse the only choice
    int correct = m_answers.GetCorrectAnswer();
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(correct)));
    Assert(button);
    button->SetHasFocus(true); // pulsing glow
  }
}

void PageMultiChoice::HideChoiceButton(int n)
{
  m_canRemoveForHint.pop_back(); // so we can't remove it again

  std::string fadeAnimName = "fade-button-" + ToString(n);
  GuiDecAnimation* fadeAnim = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, fadeAnimName));
  // Change value of this anim from zero to one, enabling descendant animations
  fadeAnim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

  // Disable the button
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(n)));
  button->SetIsEnabled(false);
}

void PageMultiChoice::ShowCorrectAnswer()
{
  int i = m_answers.GetCorrectAnswer();

  // Show a circular line drawing which fits the button for this choice.
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
  Assert(button);

  GuiElement* correct = GetElementByName(m_gui, "animate-correct");
  correct->SetLocalPos(button->GetLocalPos());
  correct->SetVisible(true);

  // Show Lurk message with explanation, if there is one.
  std::string expl = GetQuestion()->GetExplanationString();
  if (expl.empty())
  {
    LurkMsg lm(Lookup("@@@Incorrect!"), 
      GetColour(COLOUR_TEXT),
      GetColour(COLOUR_INCORRECT),
      AMJU_TOP, PAGE_LURK_TIME);
    TheLurker::Instance()->Queue(lm);

    SendNextPageMessage();
  }
  else
  {
    // TODO Red BG ?
    // Don't go to next page until user dismisses this message 
    LurkMsg lm(expl, 
      GetColour(COLOUR_TEXT), 
      GetColour(COLOUR_EXPLANATION),
      AMJU_CENTRE);
    // Set completion function to go to next page
    lm.SetOkCommand(OnDismissedExplanation);
    TheLurker::Instance()->Queue(lm);
  }
}

void PageMultiChoice::SetAnswerType(AnswerType at)
{
  m_aType = at;
}

void PageMultiChoice::SetUpQuestion()
{
  MusicalTermQuestion* q = new MusicalTermQuestion;
  SetQuestion(q);

  q->SetDictionary(GetDictionary());

  // Not great, but this stops us getting the same answer twice.
  // A better solution to this is a bit of a rewrite :(
  static std::string prevAnswer;
  int loopCount = 0; // Break out if we keep generating Qs with the same answer
  const int MAX_LOOPS = 10;
  do
  {
    q->MakeQuestion();
  } 
  while (q->GetAnswerString() == prevAnswer && loopCount++ < MAX_LOOPS);
  // Keep generating questions until we get a different answer to last time.
  
  // Store the answer for next time.
  prevAnswer = q->GetAnswerString();

  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = q->QuestionSeenBefore(m_config);
  // Set flag for next time
  q->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();
}

}

