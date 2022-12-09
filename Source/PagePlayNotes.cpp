// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Dictionary.h"
#include "DictionaryPickQuestion.h"
#include "GSPages.h"
#include "GuiMusicKb.h"
#include "GuiMusicScore.h"
#include "PagePlayNotes.h"
#include "PlayMidi.h"
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

  // Set the question type: we pick a line at random from a dictionary.
  DictionaryPickQuestion* q = new DictionaryPickQuestion;
  SetQuestion(q);

  q->SetDictionary(GetDictionary());
  q->MakeQuestion(); // pick the random line

  SetUpQuestionUI();

  // Play the note
  int note = ToInt(GetQuestion()->GetAnswerString());
  PlayMidi(note, MIDI_NOTE_MAX_VOLUME);
  // TODO Timed message to stop the note? Or just let it decay.

  m_playerHasHitNote = false;

  // Set up hint notes
  m_hintNotes.clear();
  // TODO Fill up m_hintNotes with all midi note values in range, except the
  //  correct note, then shuffle.
  //std::iota()
}

void PagePlayNotes::OnHint()
{
  // Get GUI keyboard
  GuiMusicKb* kb = dynamic_cast<GuiMusicKb*>(GetElementByName(m_gui, "kb"));
  if (!kb)
  {
    return;
  }

  static int s = 60;
  GuiMusicKb::PKey key = kb->GetKey(s++);
  if (key)
  {
    key->m_colour = Colour(1, 0, 0, 1);
  }
}

void PagePlayNotes::ShowCorrectAnswer()
{
}

void PagePlayNotes::OnMusicKbEvent(const MusicKbEvent& event)
{
  if (m_playerHasHitNote)
  {
    return;
  }
  m_playerHasHitNote = true;

  // Check note
  if (event.m_on) // note down
  {
    // Check note against answer string
    int correctNote = ToInt(GetQuestion()->GetAnswerString());
    if (correctNote == event.m_note)
    {
      dynamic_cast<GSPages*>(m_gs)->OnCorrect(Vec2f(0, 0));
    }
    else
    {
      dynamic_cast<GSPages*>(m_gs)->OnIncorrect(Vec2f(0, 0));
    }
  }
}
}

