// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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

protected:
  void SetUpButtons();
  void HideChoiceButton(int n);
//  void SetUpQuestionUI();
//  void SetQText();
//  void SetQScore();
  void SetUpQuestion();
  void InvertChoice(int c);

protected:
  MultiChoice m_answers;

  // Answer buttons we can remove when user requests a hint 
  std::vector<int> m_canRemoveForHint;

////  QuestionType m_qType = QuestionType::QTYPE_TEXT;
  AnswerType m_aType = AnswerType::ATYPE_TEXT;
};
}

