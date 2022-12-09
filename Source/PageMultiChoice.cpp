// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <functional>
#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <Localise.h>
#include <ReportError.h>
#include "CentreMsg.h"
#include "Consts.h"
#include "Dictionary.h"
#include "GSPages.h"
#include "GuiMusicScore.h"
#include "MusicalTermQuestion.h"
#include "PageMultiChoice.h"
#include "PlayWav.h"
#include "QuestionProgress.h"

namespace Amju
{
// TODO Move this somewhere else?
static PGuiElement MakeExplGui(const std::string& expl)
{
  PGuiElement gui;
  // If string ends ".txt", it's a GUI filename. Otherwise, it's some text.
  if (GetFileExt(expl) == "txt")
  {
    gui = LoadGui(expl);
  }
  else
  {
    gui = CentreMsg::MakeGuiText(expl, GetColour(COLOUR_TEXT));
  }
  Assert(gui);
  return gui;
  return gui;
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

  //GuiElement* correct = GetElementByName(m_gui, "animate-correct");
  //correct->SetVisible(false);
}

void PageMultiChoice::SetUpButtons()
{
  // Set button callbacks
  int numChoices = m_answers.GetNumAnswers();
  int correct = m_answers.GetCorrectAnswer();
//  bool seen = m_question->QuestionSeenBefore(m_config);

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
    else if (numChoices == 1)
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

void PageMultiChoice::InvertChoice(int c)
{
  // TODO All GuiElements should have SetFgCol(), these are overrides
  // TODO TEMP TEST colour
  Colour fg(1, 1, 1, 1);
  Colour bg(0, 0, 0, 1);

  GuiText* text = dynamic_cast<GuiText*>(m_gui->GetElementByName("text-choice-" + ToString(c)));
  if (text)
  {
    text->SetFgCol(fg);
  }
  else
  {
    // Set music score colour
    GuiMusicScore* score = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score-" + ToString(c)));
    Assert(score);
    score->SetFgCol(fg);
  }

  // Bg image colour
  GuiButton* button = dynamic_cast<GuiButton*>(
    m_gui->GetElementByName("button-choice-" + ToString(c)));
  Assert(button);
  button->SetButtonColour(bg);
}

void PageMultiChoice::OnChoice(int c)
{
  // Disable all buttons. 
  int numChoices = m_answers.GetNumAnswers();
  for (int i = 0; i < numChoices; i++)
  {
    GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(i)));
    button->SetIsEnabled(false);
  }

  // Notify Question GUI player has chosen
  m_pageQuestion->OnPlayerChoice();

  // Do this whether we were correct or incorrect
  HighlightCorrectAnswerButton();

  if (m_answers.IsAnswerCorrect(c))
  {
    GetPagesState()->OnCorrect();
  }
  else
  {
    GetPagesState()->OnIncorrect();
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

void PageMultiChoice::HighlightCorrectAnswerButton()
{
  // Fade all except correct
  while (CanGetHint())
  {
    OnHint();
  }

  // Highlight correct choice
  int correct = m_answers.GetCorrectAnswer();
  InvertChoice(correct);
}

void PageMultiChoice::ShowCorrectAnswer()
{
  int correct = m_answers.GetCorrectAnswer();
  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "button-choice-" + ToString(correct)));
  button->SetIsEnabled(false);

  //// Player must then hit the remaining, correct, answer
  //// Change button command - unusual, zap the previous command
  //button->SetCommand(PGuiCommand(nullptr));
  //button->SetCommand([](GuiElement*)
  //{
  //  PlayWav(WAV_CORRECT); Page::SendNextPageMessage();
  //});

  // Show Lurk message with explanation, if there is one.
  std::string expl = GetQuestion()->GetExplanationString();
  if (expl.empty())
  {
    PLurkMsg lm = new LurkMsg(Lookup("$$$122" /* Incorrect! */), 
      GetColour(COLOUR_TEXT),
      GetColour(COLOUR_INCORRECT),
      AMJU_TOP, PAGE_LURK_TIME);
    TheLurker::Instance()->Queue(lm);

    Page::SendNextPageMessage();
  }
  else
  {
    // Function to set correct answer button to go to next page and 
    //  take back focus - centred msg GUI will take focus away.
    std::function<void(GuiElement*)> okFunc = [button](GuiElement*) 
    { 
      button->SetIsFocusButton(true); 
      Page::SendNextPageMessage();
    };

    PGuiElement explGui = MakeExplGui(expl);

    // Don't go to next page until user dismisses this message 
    CentreMsg* msg = new CentreMsg;
    msg->SetCentred(
      explGui, 
      GetColour(COLOUR_TEXT), 
      GetColour(COLOUR_EXPLANATION),
      AMJU_LURK_NO_TIMER,
      okFunc);

    TheLurker::Instance()->Queue(msg);
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

  QuestionProgress& qp = GetPagesState()->GetProgress();

  q->MakeQuestion(qp);

  m_answers = q->GetMultiChoiceAnswers();

  m_canRemoveForHint.clear();
}

}

