// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <string>
#include <vector>
#include <RCPtr.h>
#include <StringUtils.h>
#include "MusicalTermsDictionary.h"

namespace Amju
{
  // There could be multiple ways to phrase the same question. E.g.
  //  What is the meaning of this Italian musical term
  //  What does this term mean? 
  //  Choose the meaning for this musical term:
  // So maybe just choose at random from a list of strings.
  // Different Q types: what is English for this (Italian etc) term,
  //  what is (Italian) term which means (English)
  // This is different data, not different behaviour.

  class Question : public RefCounted
  {
  public:
    // Get a question string at random
    // TODO what about testing
    virtual std::string GetQuestionString();

    //virtual bool Load(File*); // load question strings

    void AddQuestionString(const std::string& qs);

    virtual void MakeQuestion() = 0;

  protected:
    Strings m_questionStrings; // typically, choose one of these at random - could be default impl of GetQString
  };

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
  };

  // TODO split into files


  class MusicalTermQuestion : public MultiChoiceQuestion
  {
  public:
    // We get this from a shuffled list, which we get from a file
    std::string GetMusicalTermText();

    virtual void MakeQuestion() override;

    void SetDictionary(MusicalTermsDictionary* dictionary);

  protected:
    std::string m_musicalTerm; 

    RCPtr<MusicalTermsDictionary> m_dictionary;
  };

  class Music;

  // Ask user to pick interval from a list, 
  // OR PLAY IT? No, but the HINT could show notes on a keyboard.
  class IntervalQuestion : public MultiChoiceQuestion
  {
    // qString will be { "What is this interval?", "Name the interval".. etc

    // We need a type which represents musical score/notation
    const Music& GetIntervalAsMusic(); // to display interval as a score. 

  };

  class PlayKeyboardQuestion : public Question
  {

  };

  class PlayTheNotesQuestion : public PlayKeyboardQuestion
  {
  };

}

