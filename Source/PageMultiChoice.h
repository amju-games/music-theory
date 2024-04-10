// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <vector>
#include "MultiChoiceQuestion.h"
#include "Page.h"

namespace Amju
{
// Types for question and answer panels
//enum class QuestionType
//{
//  QTYPE_TEXT,
//  QTYPE_SCORE
//};

enum class AnswerType
{
  ATYPE_TEXT,
  ATYPE_SCORE
};

// * PageMultiChoice *
// User selects correct answer. How user selects (drag, button, etc) 
//  does not affect the core Page definition, and could be randomised.
class PageMultiChoice : public Page
{
public:
  virtual void OnActive() override;

  // Called when user makes a choice
  void OnChoice(int c);

  virtual void OnHint() override;
  virtual bool CanGetHint() override;
  virtual void ShowCorrectAnswer() override;

////  void SetQuestionType(QuestionType);
  void SetAnswerType(AnswerType);

  // Invert choice, and hide all incorrect answers
  void HighlightCorrectAnswerButton();

  // For buttons arranged in a single row, set this to true, and
  //  we will centre the buttons if there are only 2 or 3 choices.
  void SetCentreChoiceButtons(bool yesCentreChoiceButtons);

protected:
  void SetUpButtons();
  void HideChoiceButton(int n);
  void SetUpQuestion();
  void InvertChoice(int c);

protected:
  MultiChoice m_answers;

  // Answer buttons we can remove when user requests a hint 
  std::vector<int> m_canRemoveForHint;

////  QuestionType m_qType = QuestionType::QTYPE_TEXT;
  AnswerType m_aType = AnswerType::ATYPE_TEXT;

  bool m_yesCentreChoiceButtons = false;
};
}

