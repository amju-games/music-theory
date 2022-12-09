// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <vector>
#include "MultiChoiceQuestion.h"
#include "Page.h"

namespace Amju
{
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
  virtual void ShowCorrectAnswer() override;

protected:
  void HideChoiceButton(int n);

protected:
  MultiChoice m_answers;

  // Answer buttons we can remove when user requests a hint 
  std::vector<int> m_canRemoveForHint;
};
}

