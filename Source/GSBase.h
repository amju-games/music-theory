// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <Game.h>
#include <GameState.h>
#include <GuiElement.h>
#include "MusicEvent.h"

namespace Amju
{
class KeyInputHandler;

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

  // This should be the one and only KeyEvent handler. 
  // All KeyEvents go though KeyInputHandler, so our job in the
  //  game states is now to add and remove handlers.
  bool OnKeyEvent(const KeyEvent&) override final;

  // Add key mappings for this state.
  // Returns the key input handler as a convenience for subclass impl.
  virtual KeyInputHandler& AddKeyInputHandlers();

  // Remove the mappings we added in the above function.
  // (Default impl clears all mappings)
  virtual void RemoveKeyInputHandlers();

  // Music events are sent to current game state:
  // Override this to handle music events.
  virtual void OnMusicKbEvent(const MusicKbEvent&) {}

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


