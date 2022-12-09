// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GuiElement.h>
#include <RCPtr.h>
#include "GSBase.h"

namespace Amju
{
// * Page *
// A page is one screenful of information, usually in the form of a "test" or question. 
// E.g. rather than just present a musical term and its meaning, we pose it as a question,
//  asking the user to choose the correct meaning. If the user is stuck, or hasn't seen 
//  this before, etc, we show the answer, but the user still chooses it.

// TODO Decouple from GUI:
// Pages talk to a GUI, but don't own it or load it directly. This is to keep the pages
//  separate from how they look, so we can present them differently, and unit test them.


// Just for now, make Pages derived from GSBase
class Page : public GSBase
{
public:
  // Load GUI, and question info independently?
  virtual void OnActive() override;

protected:
  virtual bool LoadQuestion() = 0;

protected:
  // Base gui file name - we append current orientation and reload if the orientation changes.
  std::string m_guiName;
};
}
