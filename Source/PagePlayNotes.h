// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "Page.h"

namespace Amju
{
// * PagePlayNotes * 
// Show note on stave. User plays the note on Keyboard.
class PagePlayNotes : public Page
{
public:
  PagePlayNotes();

  virtual void OnActive() override;
  virtual void OnHint() override;
  virtual void ShowCorrectAnswer() override;

};
}
