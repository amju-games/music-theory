// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "Page.h"

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

  class Question
  {
    Strings m_qStrings; // typically, choose one of these at random - could be default impl of GetQString

    virtual std::string GetQuestionString() = 0;
  };

  public MultiChoiceQuestion : public Question
  {
  public:
    // The answer is tightly coupled, NOT a separate object.

    // Get (randomised?) answers to present to user, who then chooses one
    //  of them.
    // Get non-shuffled for testing
    Strings GetMultiChoiceAnswers(bool shuffled = true);

    int GetCorrectAnswer(); // or index into answers

    bool IsAnswerCorrect(int n); // index into answers

  };


  class MusicalTermQuestion : public MultiChoiceQuestion
  {
  public:
    // We get this from a shuffled list, which we get from a file, right? 
    std::string GetMusicalTermText();

  protected:
    std::string m_musicalTerm; // shown in different style I should think
  };

  // Ask user to pick interval from a list, 
  // OR PLAY IT? No, but the HINT could show notes on a keyboard.
  class IntervalQuestion : public MultiChoiceQuestion
  {
    // qString will be { "What is this interval?", "Name the interval".. etc

    // We need a type which represents musical score/notation
    const MusicalNotation& GetIntervalAsNotation(); // to display interval as a score. 

  };

  class PlayTheNotesQuestion : public PlayKeyboardQuestion
  {
  };


  // HINTS are intensely GUI-based, animations/behaviour rather than 
  //  just data like strings. So an active Hint takes over the GUI, and
  //  can load its own GUI, which it displays in addition to the 
  //  standard GUI.
  // Multi coloured highlighting: this will be much easier if vertices
  //  have colours. Then we don't have multiple scores, say, or snippets
  //  of text. It's all the same tri list, just with different vertex
  //  colours. OK MAKE IT SO.




  // * PageMultiChoice *
  // User selects correct answer. How user selects (drag, button, etc) 
  //  does not affect the core Page definition, and could be randomised.
  class PageMultiChoice : public Page
  {
  public:


  protected:

  };


  // TODO TEMP TEST
  // Sketching out how this would work
  class PageMusicalTerm : public PageMultiChoice
  {
  public:
    

  protected:
    Question* q;
    Hints hints;
    
    std::string m_
  };

  

}

