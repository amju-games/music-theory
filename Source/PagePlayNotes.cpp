// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <numeric> // iota()
#include <Localise.h>
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

GuiMusicKb* PagePlayNotes::GetKb()
{
  auto kb = dynamic_cast<GuiMusicKb*>(GetElementByName(m_gui, "kb"));
  Assert(kb);
  return kb;
}

void PagePlayNotes::SetKbPos(const Vec2f& p)
{
  auto kb = GetElementByName(m_gui, "kb-comp");
  Assert(kb);
  Vec2f pos = kb->GetLocalPos();
  pos += p;
  kb->SetLocalPos(pos);
}

bool PagePlayNotes::Load(const Strings& strs)
{
  if (!Page::Load(strs))
  {
    return false;
  }

  // For this page type, the 3rd string is the keyboard x-offset, i.e. the
  //  screen position of middle C.
  Assert(strs.size() > 2);
  float kbXPos = ToFloat(strs[2]);
  m_kbPos = Vec2f(kbXPos, 0);

  // 4th string is instruction text for this Page type
  Assert(strs.size() > 3);
  m_instructionText = Lookup(strs[3]);

  return true;
}

void PagePlayNotes::OnActive()
{
  Page::OnActive();

  SetKbPos(m_kbPos);

  // Set the question type: we pick a line at random from a dictionary.
  DictionaryPickQuestion* q = new DictionaryPickQuestion;
  SetQuestion(q);

  q->SetDictionary(GetDictionary());
  q->MakeQuestion(); // pick the random line

  SetUpQuestionUI();

  // Play the note
  int correctNote = ToInt(GetQuestion()->GetAnswerString());
  PlayMidi(correctNote, MIDI_NOTE_MAX_VOLUME);
  // TODO Timed message to stop the note? Or just let it decay.

  m_playerHasHitNote = false;

  // Set up hint notes
  m_hintNotes.clear();
  GuiMusicKb* kb = GetKb();
  // Get number of midi notes/keys in the keyboard
  int minKey = kb->GetMinKey();
  int maxKey = kb->GetMaxKey();
  int numKeys = maxKey - minKey + 1; // inclusive count of all notes
  m_hintNotes.resize(numKeys);
  // Fill up m_hintNotes with all midi note values in range, except the
  //  correct note, then shuffle.
  std::iota(m_hintNotes.begin(), m_hintNotes.end(), minKey);
  // Remove correct note from hints
  m_hintNotes.erase(std::remove(m_hintNotes.begin(), m_hintNotes.end(), correctNote),
    m_hintNotes.end());
  std::random_shuffle(m_hintNotes.begin(), m_hintNotes.end());
}

bool PagePlayNotes::CanGetHint()
{
  return !m_hintNotes.empty();
}

void PagePlayNotes::OnHint()
{
  // Get GUI keyboard
  GuiMusicKb* kb = GetKb();
  if (!kb)
  {
    return;
  }

  // Choose some hint notes - then erase them so we can't choose them again.
  const int NUM_HINTS = 3; // maybe depends on kb length?
  for (int i = 0; i < NUM_HINTS && !m_hintNotes.empty(); i++)
  {
    int s = m_hintNotes.back();
    m_hintNotes.pop_back();
    GuiMusicKb::PKey key = kb->GetKey(s);
    if (key)
    {
      // TODO Colour black and white keys separately
      key->m_colour = Colour(1, 0, 0, 1);
    }
  }
}

void PagePlayNotes::ShowCorrectAnswer()
{
  // Get GUI keyboard
  GuiMusicKb* kb = GetKb();
  if (!kb)
  {
    return;
  }
  // Set colour of correct note
  int correctNote = ToInt(GetQuestion()->GetAnswerString());
  GuiMusicKb::PKey key = kb->GetKey(correctNote);
  if (key)
  {
    // TODO Colour black and white keys separately
    key->m_colour = Colour(0, 1, 0, 1);
  }

  // TODO Explanation? E.g. show "F-A-C-E" etc??
  SendNextPageMessage();
}

void PagePlayNotes::OnMusicKbEvent(const MusicKbEvent& event)
{
  if (m_playerHasHitNote)
  {
    return;
  }

  // Check note
  if (event.m_on) // note down
  {
    m_playerHasHitNote = true;
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

