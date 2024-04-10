// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

// Experimental types for defining a Question, Answer and Hint on a page.

namespace Amju
{
class Panel
{
public:
  virtual ~Panel() {}
  virtual void OnActive() {};
  virtual void OnDeactive() {};

};

class AnswerPanel : public Panel
{
};

class QuestionPanel : public Panel
{
};

// One page has a series of hints, which are unlocked one at a time. The final hint gives the answer.
// Also for new question types, we should show the answer.
// Hints could affect the appearance of the Question and the Answer!
// E.g.: to show interval, we may want to highlight a note on the stave (Question) while showing
//  the same note on a keyboard in the Hint area.
// For MCQ, we might want to pulse the correct answer..?
// So we can't split up the Page into Question/Answer/Hint..?
// Just keep protoyping, design is not clear to me yet.
class HintPanel : public Panel
{
};


}

