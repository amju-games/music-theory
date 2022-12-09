// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Game.h>
#include <GameState.h>
#include <GuiElement.h>

namespace Amju
{
// * GSBase *
// Base class for game states for Amjula music theory
class GSBase : public GameState
{
public:
  virtual void Update() override;
  virtual void Draw() override {}
  virtual void Draw2d() override;
  virtual void OnActive() override;
  virtual void OnDeactive() override;

  // Reload
  virtual bool OnKeyEvent(const KeyEvent&) override;

  GuiElement* GetGui();

  // Call when we animate to a new state etc, preventing user pressing the 
  //  same button again, or a different button.
  // Return this, so we can hide buttons as part of chain of calls, for
  //  convenience.
  virtual GSBase* HideButtons();

  // Scroll screen in given direction, using animation + translation decorators.
  // Assumes the gui page has the correctly named animation element.
  // This must be "scroll-up-trigger", "scroll-left-trigger", etc.
  void ScrollLeft();
  void ScrollRight();
  void ScrollUp();
  void ScrollDown();

protected:
  void HideButtons(GuiElement*);

  // For dev/debugging, or when landscape/portrait orientation changes.
  // Default impl is to call OnDeactive() then OnActive, so reloading everything
  //  for the state. (This could have unwanted effects.)
  virtual void ReloadGui();

protected: 
  // GUI - this is a 2D app
  PGuiElement m_gui;
  std::string m_guiFilename; // used to load m_gui - set this in ctor etc.
};

// Useful function for timed messages
template<class T>
void GoTo()
{
  TheGame::Instance()->SetCurrentState(T::Instance());
}
}


