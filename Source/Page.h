// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GuiElement.h>
#include <RCPtr.h>
#include "Question.h"

namespace Amju
{
class ConfigFile;
class GameState;

// * Page *
// A page is one screenful of information, usually in the form of a "test" or question. 
// E.g. rather than just present a musical term and its meaning, we pose it as a question,
//  asking the user to choose the correct meaning. If the user is stuck, or hasn't seen 
//  this before, etc, we show the answer, but the user still chooses it.

// TODO Decouple from GUI:
// Pages talk to a GUI, but don't own it or load it directly. This is to keep the pages
//  separate from how they look, so we can present them differently, and unit test them.
// OK, QUESTION is the non-GUI question, Page presents a question in a GUI.

// Maybe have a "GuiMaker" which makes the Gui for a given question type

// We want to display one page, then another, with some transition between them, e.g.
//  swipe old page off screen.

class Page : public RefCounted
{
public:
  // Load GUI, and question info independently?
  virtual void OnActive();
  virtual void Draw();
  virtual void Update();

  // Set config file to save any state we want to remember about this page/topic
  void SetConfigFile(ConfigFile* cf);

  void SetGameState(GameState* gs);

  // Called when user taps Hint button -- show page-specific hint.
  virtual void OnHint() {}
  virtual void ShowCorrectAnswer() {}

  // Sigh, there is a difference in behaviour if we are showing something new, so
  //  set this flag if user is learning a new thing.
  void SetIsLearnMode(bool learn);
  bool IsLearnMode() const;

protected:
//?  virtual bool LoadQuestion() = 0;

protected:
  // Base gui file name - we append current orientation and reload if the orientation changes.
  std::string m_guiName;

  PGuiElement m_gui;

  RCPtr<Question> m_question;

  GameState* m_gs = nullptr;

  bool m_isLearnMode = false;

  // Config file to save any state we want to remember about this page/topic
  RCPtr<ConfigFile> m_config;
};
}
