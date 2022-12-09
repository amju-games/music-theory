// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GuiMusicScore.h"
#include "PagePlayNotes.h"
#include "ScoreBuilder.h"

namespace Amju
{
PagePlayNotes::PagePlayNotes()
{
  m_guiName = "play_notes";
}

void PagePlayNotes::OnActive() 
{
  Page::OnActive();

  // Create "question" - this should be external, right?
  // Pick clef
  // Pick note - how to display it, e.g. octave, enharmonic?

  // Add the chosen note to the stave already set up in gui file
  ScoreBuilder sb;
  int pitch = 0; // TODO
  int x = 128; // TODO
  int y = 0; // TODO
  int bar = 1;
  Note note(pitch, x, y, Note::SEMIBREVE, bar);
  sb.Add(note);

  GuiMusicScore* score = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "score"));
  Assert(score);
  sb.Write(*score);
}

void PagePlayNotes::OnHint() 
{
}

void PagePlayNotes::ShowCorrectAnswer() 
{
}

}

