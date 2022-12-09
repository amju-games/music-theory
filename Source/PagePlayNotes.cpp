// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "Dictionary.h"
#include "DictionaryPickQuestion.h"
#include "GSPages.h"
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
}

void PagePlayNotes::OnHint()
{
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

