// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "Question.h"

namespace Amju
{
// * MultiChoice *
// Stores the answer strings, and the index of the correct answer.
class MultiChoice
{
public:
  int GetCorrectAnswer() const; // or index into answers
  bool IsAnswerCorrect(int n) const; // index into answers
  void AddAnswer(const std::string& answer);
  void SetCorrectAnswer(int correct);
  int GetNumAnswers() const;
  const std::string& GetAnswer(int n) const;

private:
  Strings m_answers;
  int m_correctAnswer = -1;
};

class MultiChoiceQuestion : public Question
{
public:
  // The answer is tightly coupled to the question, NOT a separate object.

  // Get (randomised?) answers to present to user, who then chooses one
  //  of them.
  // Get non-shuffled for testing
  MultiChoice GetMultiChoiceAnswers();

protected:
  MultiChoice m_answers;

  // Set if we swap the question and answer strings for variety
  bool m_qAndASwitched = false;
};

}
