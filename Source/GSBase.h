// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GameState.h>
#include <GuiElement.h>

namespace Amju
{
// * GSBase *
// Base class for game states for Amjula music theory
class GSBase : public GameState
{
public:
  virtual void Draw() override {}
  virtual void Draw2d() override;
  virtual void OnActive() override;
  virtual void OnDeactive() override;

  // Reload
  virtual bool OnKeyEvent(const KeyEvent&) override;

protected: 
  // GUI - this is a 2D app
  PGuiElement m_gui;

};
}


