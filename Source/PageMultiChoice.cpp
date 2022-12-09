// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include <GuiDecAnimation.h>
#include <ReportError.h>
#include "Dictionary.h"
#include "GSPages.h"
#include "GuiMusicScore.h"
#include "MusicalTermQuestion.h"
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
  SetUpQuestion(); // decide on Q to ask, and real + false answers

  Page::OnActive();

  SetUpButtons();

  SetUpQuestionUI();
}

void PageMultiChoice::SetUpQuestionUI()
{
  // Hmm, switch on type, refactor as subclasses
  switch (m_qType)
  {
  case QuestionType::QTYPE_SCORE:
    SetQScore();
    break;
  case QuestionType::QTYPE_TEXT:
    SetQText();
    break;
  }
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
      button->SetText(answerStr);
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

void PageMultiChoice::SetQuestionType(QuestionType qt)
{
  m_qType = qt;
}

void PageMultiChoice::SetAnswerType(AnswerType at)
{
  m_aType = at;
}

void PageMultiChoice::SetUpQuestion()
{
  MusicalTermQuestion* q = new MusicalTermQuestion;
  m_question = q;

  // Get musical terms dictionary - this is a Resource.
  Dictionary* dic = dynamic_cast<Dictionary*>(
    TheResourceManager::Instance()->GetRes(m_dictionaryFilename));
  Assert(dic);

  q->SetDictionary(dic);
  q->MakeQuestion();
  m_answers = q->GetMultiChoiceAnswers();

  // Seen this question before? If not we will give the correct answer.
  bool seen = q->QuestionSeenBefore(m_config);
  // Set flag for next time
  q->SetQuestionSeenBefore(m_config);

  m_canRemoveForHint.clear();
}

void PageMultiChoice::SetQText()
{
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "musical-term-text"));
  Assert(text);
  text->SetText(m_question->GetQuestionString());
}

void PageMultiChoice::SetQScore()
{
  // Set musical score display from question text
  GuiMusicScore* ms = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score"));
  Assert(ms);
  // Simple: multiple glyphs. TODO smart ScoreBuilder.
  // Multiple glyphs split by ;
  Strings strs = Split(m_question->GetQuestionString(), ';');
  for (const std::string& s : strs)
  {
    if (!ms->AddMultipleGlyphsFromString(s))
    {
      ReportError("Failed to set score glyph: " + s);
      Assert(0);
    }
  }

  // TODO Ideally we would like to do this:
  //ScoreBuilder sb;
  //std::string error;
  //bool ok = sb.SetFromString(q->GetQuestionString(), &error);
  //if (!ok)
  //{
  //  std::cout << "Error in score string: " << error << "\n";
  //  Assert(0);
  //}
  //sb.Write(*ms);
}

}

