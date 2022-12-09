// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSPages.h"
#include "GuiMusicScore.h"
#include "PagePlayNotes.h"
#include "ScoreBuilder.h"

namespace Amju
{
const char* PagePlayNotes::NAME = "page-play-notes";

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

  // Choose pitch depending on if we are testing accidentals etc?
  int pitch = 0; // TODO
  int x = 192; // TODO
  int y = rand() % 12 - 6; // TODO
  int bar = 1;
  Note note(pitch, x, y, Note::SEMIBREVE, bar);
  sb.Add(note);

  // Tell ScoreBuilder which clef to use, and the pitch, then 
  //  it works out the y-pos. So clef not hard coded in gui layout?

  // Tell S.B. the key sig?? (Guido has +/- number of sharps/flats)

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

void PagePlayNotes::OnMusicKbEvent(const MusicKbEvent& event)
{
  // Check note
  if (!event.m_on)
  {
    dynamic_cast<GSPages*>(m_gs)->OnCorrect();
  }
}
}

