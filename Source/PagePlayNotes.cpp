// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <numeric> // iota()
#include <Localise.h>
#include "CentreMsg.h"
#include "Dictionary.h"
#include "DictionaryPickQuestion.h"
#include "GSPages.h"
#include "GuiMusicKb.h"
#include "GuiMusicScore.h"
#include "PagePlayNotes.h"
#include "PlayMidi.h"
#include "QuestionProgress.h"
#include "ScoreBuilder.h"

namespace 
{
// Convert string of comma-separated ints into a vector of ints.
std::vector<int> ParseIntSeq(const std::string& seq)
{
  Amju::Strings strs = Amju::Split(seq, ',');
  std::vector<int> res;
  res.reserve(strs.size());
  for (const std::string& s : strs)
  {
    res.push_back(Amju::ToInt(s));
  }
  return res;
}
} // anon namespace

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

  QuestionProgress& qp = GetPagesState()->GetProgress();
  q->MakeQuestion(qp); // pick the random line

  SetUpQuestionUI();

  m_correctSequence = ParseIntSeq(GetQuestion()->GetAnswerString());
  m_correctSequenceCurrentPos = 0;

  // Don't play the note - player can play it if they would like to.
  // Plus, now it's a sequence, not an individual note.
  //PlayMidi(correctNote, MIDI_NOTE_MAX_VOLUME);
  // TODO Timed message to stop the note? Or just let it decay.

  m_playerHasHitNote = false;

  // Set up hint notes
  m_hintNotes.clear();
#ifdef YES_HINT
  GuiMusicKb* kb = GetKb();
  // Get number of midi notes/keys in the keyboard
  int minKey = kb->GetMinKey();
  int maxKey = kb->GetMaxKey();
  int numKeys = maxKey - minKey + 1; // inclusive count of all notes
  m_hintNotes.resize(numKeys);
  // Fill up m_hintNotes with all midi note values in range, except the
  //  correct notes, then shuffle.
  std::iota(m_hintNotes.begin(), m_hintNotes.end(), minKey);
  // Remove correct notes from hints - TODO
//  m_hintNotes.erase(std::remove(m_hintNotes.begin(), m_hintNotes.end(), correctNote),
    m_hintNotes.end());
  std::random_shuffle(m_hintNotes.begin(), m_hintNotes.end());
#endif
}

bool PagePlayNotes::CanGetHint()
{
  return false;
//  return !m_hintNotes.empty();
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
  // TODO This only shows the first note of a multi-note sequence
  int correctNote = m_correctSequence[0];
  GuiMusicKb::PKey key = kb->GetKey(correctNote);
  if (key)
  {
    // TODO Colour black and white keys separately
    key->m_colour = Colour(0, 1, 0, 1);
  }
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
    PlayMidi(event.m_note, MIDI_NOTE_MAX_VOLUME);
std::cout << "Playing note " << event.m_note << "\n";

    GuiMusicKb* kb = GetKb();
    GuiMusicKb::PKey key = kb->GetKey(event.m_note);

    // Check note against answer string
    int correctNote = m_correctSequence[m_correctSequenceCurrentPos];
    if (correctNote == event.m_note)
    {
std::cout << "Note " << m_correctSequenceCurrentPos << " correct!\n";

      // Correct! Show something/play wav?
      // TODO Colour black and white keys separately
      key->m_colour = Colour(0, 1, 0, 1);

      m_correctSequenceCurrentPos++;

      if (m_correctSequenceCurrentPos == m_correctSequence.size())
      {
std::cout << "All notes played correctly!\n";

        m_playerHasHitNote = true;

        // Would be good to delay these calls for ~.5s, so we first hear 
        //  the notes the player played, before we hear the good/bad sound.
        GetPagesState()->OnCorrect();
      }
    }
    else
    {
std::cout << "Note " << m_correctSequenceCurrentPos << " incorrect!\n";

      m_playerHasHitNote = true;
      key->m_colour = Colour(1, 0, 0, 1);
      GetPagesState()->OnIncorrect();
      ShowExplanation(correctNote, event.m_note);
    }
  }
}

void PagePlayNotes::ShowExplanation(int correctNote, int badNote)
{
  // Composer-specific text, then 
  // explanation - "you played an A, but you should have played a C"
  std::string composerNo = GetPagesState()->GetIncorrectStr();
  const int BUF_SIZE = 1000;
  const std::string NOTES[] =
  {
    Lookup("@@@a C"), // TODO LOCALISE ALL
    "a C sharp",
    "a D",
    "a D sharp",
    "an E",
    "an F",
    "an F sharp",
    "a G",
    "a G sharp",
    "an A",
    "a B flat",
    "a B"
  };
  std::string badNoteStr = NOTES[badNote % 12];
  std::string goodNoteStr = NOTES[correctNote % 12];
  char buf[BUF_SIZE];
  // Add space between composer No string and explanation string.
  std::string formatStr = " " + Lookup("@@@You played %s but you should have played %s!");
  // Looked-up string should contain two %s, first for bad note, second for good note.
  // TODO Allow %s1, %s2, so we can flip them if reuqired by the current language.
  sprintf(buf, formatStr.c_str(), badNoteStr.c_str(), goodNoteStr.c_str());
  std::string expl = composerNo + buf;

  PLurkMsg lm = new CentreMsg(expl,
    Colour(0, 0, 0, 1), // text colour,
    Colour(1, 1, 1, 1), // bg colour,
    AMJU_LURK_NO_TIMER,
    [](GuiElement*) { SendNextPageMessage(); } // go to next page when player clicks OK
  );
    // TODO Speech bubble arrow flag/enum
  TheLurker::Instance()->Queue(lm);
}
}

