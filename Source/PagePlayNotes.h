// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "Page.h"

namespace Amju
{
class GuiMusicKb;
  
// * PagePlayNotes * 
// Show note on stave. User plays the note on Keyboard.
class PagePlayNotes : public Page
{
public:
  PagePlayNotes();

  static const char* NAME;

  virtual void OnActive() override;
  virtual void OnHint() override;
  virtual bool CanGetHint() override;
  virtual void ShowCorrectAnswer() override;
  virtual void OnMusicKbEvent(const MusicKbEvent&) override;

private:
  GuiMusicKb* GetKb();

private:
  // Player only gets to play one note per page
  bool m_playerHasHitNote = false;

  // Set of midi note values which we can show is NOT the correct answer.
  // This obviously excludes the correct note. As we highlight incorrect
  //  notes, we remove them from this set.
  std::vector<int> m_hintNotes;
};
}
