// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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

  // Check keys for debug features: R to reload, G to show GUI tree
  virtual bool OnKeyEvent(const KeyEvent&) override;

  GuiElement* GetGui();

  const std::string& GetGuiFilename();

  // Call when we animate to a new state etc, preventing user pressing the 
  //  same button again, or a different button.
  // Return this, so we can hide buttons as part of chain of calls, for
  //  convenience.
  virtual GSBase* HideButtons();

protected:
  void HideButtons(GuiElement*);

  // For dev/debugging, or when landscape/portrait orientation changes.
  // Default impl is to call OnDeactive() then OnActive, so reloading everything
  //  for the state. (This could have unwanted effects.)
  virtual void ReloadGui();

  // If B key pressed, go to previous state: return true if we do.
  bool CheckForKey_B_BackToPrevState(const KeyEvent& ke);

  void DrawDevMenu();

protected: 
  // 2D GUI
  PGuiElement m_gui;
  std::string m_guiFilename; // used to load m_gui - set this in ctor 
};

// Useful function for timed messages: go to Game State T.
template<class T>
void GoTo()
{
  TheGame::Instance()->SetCurrentState(T::Instance());
}

// Command for "Share" buttons for Twitter etc
void OnShare(GuiElement*);

} // namespace


