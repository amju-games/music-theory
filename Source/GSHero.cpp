// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <iostream>
#include <CommandLineArgs.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include <SoundManager.h>
#include "Consts.h"
#include "Grader.h"
#include "HeroGameRound.h"
#include "GSHero.h"
#include "GSHeroEnd.h"
#include "GSHeroWin.h"
#include "NumUpdate.h"
#include "PlayWav.h"

//#define KEYBOARD_DEBUG

namespace Amju
{
static int IsWhite(int midi)
{
  int mod = midi % 12;
  return mod != 1 && mod != 3 && mod != 6 && mod != 8 && mod != 10;
}

// Count the white notes in the given range of midi notes. Inclusive range.
static int CountWhiteNotes(int midi1, int midi2)
{
  Assert(midi1 <= midi2);
  int white = 0;
  while (midi1 <= midi2)
  {
    white += IsWhite(midi1);
    midi1++;
  }
  return white;
}

static void OnNoteEvent(const NoteEvent& ne)
{
  TheGSHero::Instance()->OnNoteEvent(ne);
}

GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

void GSHero::UpdateKeyboardPosition()
{
  // Set keyboard x-coord so that upcoming notes will be playable.

  // Look ahead to see what note events will be coming soon.
  const auto& noteEvents = m_scrollScore->GetNoteEvents();
  float animTime = m_scrollScore->GetAnimTime();

  // Get all notes which will occur after the current anim time.
  auto it = std::upper_bound(noteEvents.begin(), noteEvents.end(), animTime,
    [](float t, const NoteEvent& ne) { return t < ne.m_time; });

  if (it == noteEvents.end())
  {
    return; // no more notes?! 
  }
 
  // Look ahead this many events. 
  const int lookAhead = 7;
  // Reduce the range if we are near the end of the note events.
  auto end = std::min(it + lookAhead, noteEvents.end());
  // Get the min and max notes in the range
  const auto [minIt, maxIt] = std::minmax_element(it, end,
    [](const NoteEvent& ne1, const NoteEvent& ne2) { return ne1.m_note < ne2.m_note; });
  const int minNote = minIt->m_note;
  const int maxNote = maxIt->m_note;

#ifdef KEYBOARD_DEBUG
std::cout << "** Look ahead: min note: " << minNote << " max note: " << maxNote << "\n";
#endif

  // Convert distance in midi note values to screen space distance in x:
  // We use the screen space width of a white key. Count how many
  //  white keys we need to move, and mult by width.

  // Just get key width once: we won't be changing this, right??
  const float keyWidth = 
    m_keyboard->GetKey(60)->m_projectedRect.GetSize().x;

#ifdef KEYBOARD_DEBUG
std::cout << "Key width: " << keyWidth << "\n";
#endif

  const auto& currentPos = m_keyboardTranslate->GetLocalPos();
  auto desiredPos = currentPos;
  const int screenMin = m_keyboard->GetMinKeyOnScreen();
  const int screenMax = m_keyboard->GetMaxKeyOnScreen();
  if (maxNote >= screenMax)
  {
    int whiteNotes = CountWhiteNotes(screenMax, maxNote);
    desiredPos.x -= keyWidth * static_cast<float>(whiteNotes);

#ifdef KEYBOARD_DEBUG
std::cout << "  Max note on screen: " << screenMax 
  << " max note coming up: " << maxNote 
  << " white notes: " << whiteNotes 
  << "\n";
#endif
  }
  else if (minNote <= screenMin) // both conditions should not be true!
  {
    int whiteNotes = CountWhiteNotes(minNote, screenMin);
    desiredPos.x += keyWidth * static_cast<float>(whiteNotes);

#ifdef KEYBOARD_DEBUG
std::cout << "  Min note on screen: " << screenMin
  << " min note coming up: " << minNote 
  << " white notes: " << whiteNotes 
  << "\n";
#endif
  }
  else
  {
    return; // no need to do anything, let the animation finish
  }

  // Copy current value to initial value, and set new desired value.
  // (Copying current to initial sounds like a useful thing to add 
  //  to the GuiDecorator class.)
  m_keyboardTranslate->SetTranslation(currentPos, 0);
  m_keyboardTranslate->SetTranslation(desiredPos, 1);
  m_keyboardAnim->ResetAnimation();
}

void GSHero::Update()
{
  GSBase::Update();

  m_playerScore.Update();
  m_lifePercent.Update();

  float animTime = m_scrollScore->GetAnimTime();
  if (!m_roundIsOver && animTime > 0.9999f)
  {
    OnPlayerHasWon();
  }
}

void GSHero::ReloadGui()
{
  GSBase::ReloadGui();

  auto sm = TheSoundManager::Instance();
  sm->StopSong();
}

static const int NUM_UPDATE_NUM_FRAMES = 50;

void GSHero::IncreaseScore(const Grade& grade)
{
  int amount = std::round(grade.m_score * 1000.f);
  amount *= 100;

  m_playerScore.Add(amount, NUM_UPDATE_NUM_FRAMES);
}

void GSHero::DecreaseLife(const Grade& grade)
{
  m_lifePercent.Add(-10, NUM_UPDATE_NUM_FRAMES);

  if (m_lifePercent.m_internalNumber <= 0)
  {
    OnPlayerHasLost();
  }
}

void GSHero::OnPlayerHasWon()
{
std::cout << "Player has won this round!\n";

  m_roundIsOver = true;

  // Go to next game state after short delay
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGSHeroWin>,
    SecondsFromNow(2.f)));
}

void GSHero::OnPlayerHasLost()
{
std::cout << "Player has lost this round!\n";

  m_roundIsOver = true;

  PlayWav("record_scratch");

  // Stop scrolling - TODO Grind to a halt, not immediate stop
  m_scoreAnim->SetIsPaused(true);

  // Go to next game state after short delay
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGSHeroEnd>,
    SecondsFromNow(2.f)));
}

const HeroGameRound& GetGameRound()
{
  auto& round = TheGameRoundManager::Instance()->GetGameRound(0);
  return round;
}

void GSHero::Start()
{
std::cout << "START!!\n";

  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac

#ifdef AMJU_USE_BASS_MIDI
  // Set sound font for this round (so there is variety in player sound, and
  //  can fit the song better)
  std::string soundFont = File::GetRoot() + GetGameRound().m_soundFont;
  bool ok = sm->MidiSetSoundFont(soundFont.c_str());
  if (!ok)
  {
    std::cout << "Bad news, couldn't set sound font.\n";
  }
#endif

  // Play count-in audio
  // The BPM has to match that of the song. We don't want a huge number of
  //  count-in audio files, so better to adjust BPM of the count-in song, to 
  //  match the main song BPM.
  sm->PlaySong(GetGameRound().m_countIn);  

  // Callback for when count-in finished
  auto onFinished = []() { TheGSHero::Instance()->OnCountInFinished(); };

  // Start the count-in
  const int numCountInBeats = GetGameRound().m_numCountInBeats;
  m_scrollScore->StartCountIn(numCountInBeats, onFinished);

  UpdateKeyboardPosition();
}

void GSHero::OnCountInFinished()
{
  // Start score animating, and set backing track playing at the same time.
  // The score and the song have to be in perfect sync.
  // TODO Is there a way to sync the song?
  m_scoreAnim->SetIsPaused(false);

  // Start playing the backing track for this song
  auto sm = TheSoundManager::Instance();
  sm->PlaySong(GetGameRound().m_backingTrack);
}

void GSHero::OnNoteEvent(const NoteEvent& ne)
{
  // This is a callback for a music note event generated by the Score.
  if (m_roundIsOver)
  {
    return;
  }

  if (ne.m_onNotOff)
  {
    // Just update this once per note, not on both note on and note off
    //  events. It doesn't really matter which event type we chose.
    UpdateKeyboardPosition();
  }

  // This is logic to detect a missed note, i.e. player does not 
  //  attempt.
  if (!ne.m_onNotOff)
  {
    // Note off: increment count of completed score-generated events
    m_numScoreNotes++;

#ifdef COUNT_NOTES_DEBUG
std::cout << "Player notes: " << m_numPlayerNotes
  << " Score notes: " << m_numScoreNotes << "\n";
#endif
  }

  if (m_numScoreNotes > m_numPlayerNotes)
  {
std::cout << "Player has missed a note, I think.\n";

    m_numPlayerNotes = m_numScoreNotes;

    // Missed note
    Grade grade(Grade::NO_ATTEMPT, 0);
    // Show this differently TODO... the problem is the feedback is at
    //  the end of a note's life, so the feedback is not connecting to 
    //  the note we missed.
//  FeedbackBalloon(grade);
    DecreaseLife(grade); 
  }
}

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
  // This is a player-generated event
  if (m_roundIsOver)
  {
    return;
  }

  if (e.m_on)
  {
    //m_noteAttempted = true;
    m_numPlayerNotes++;

#ifdef COUNT_NOTES_DEBUG
std::cout << "Player notes: " << m_numPlayerNotes
  << " Score notes: " << m_numScoreNotes << "\n";
#endif
  }
  GradeEvent(e);
}

void GSHero::GradeEvent(const MusicKbEvent& e)
{
#ifdef GRADE_DEBUG
std::cout << "=================================\nGrading note event: Pitch: " << e.m_note 
  << " " << (e.m_on ? "*ON*" : "+off+");
  // No newline!
#endif

  // Get song length
  auto optSongLength = m_scrollScore->GetSongLengthSeconds();
  // If we got here, we surely know the song length!
  if (!optSongLength)
  {
#ifdef GRADE_DEBUG
std::cout << "\n";
#endif
    std::cout << "*** Unexpected, can't get song length!!\n";
    return;
  }
  float songLength = *optSongLength;

  float animTime = m_scrollScore->GetAnimTime();
  if (animTime >= 1.f)
  {
#ifdef GRADE_DEBUG
std::cout << "\n";
#endif
    // Ignore note down event after song finished. But allow for final
    //  late note up event??
    if (e.m_on) // ? Or safer to just totally ignore
    {
      return;
    }
  }
  else if (animTime == 0)
  {
    // We haven't started the song yet. 
    // But this could be a slightly early attempt at the first note
    //  in the song.
    // So we grade as usual, but using current count-in time remaining.
    float timeBeforeSecs = m_scrollScore->GetCountInTimeRemaining();
    if (timeBeforeSecs == 0)
    {
      // If we haven't even started the count-in, ignore this event.
#ifdef GRADE_DEBUG
std::cout << "  * not even counting in yet bruv!\n";
#endif
      return;
    }
#ifdef GRADE_DEBUG
std::cout << " -- grade count-in event!\n";
#endif
    // Convert time in seconds to normalised time
    animTime = - timeBeforeSecs / songLength;
#ifdef GRADE_DEBUG
std::cout << "  - time before start: " << timeBeforeSecs << " normalised: " << animTime << "\n";
#endif
  }

#ifdef GRADE_DEBUG
std::cout << " AnimTime now: " << animTime 
  << "...\n";
#endif

  const auto& noteEvents = m_scrollScore->GetNoteEvents();

  // Get iterator pointing to the event we think the player is attempting.
  Grader grader;
  auto optIt = grader.GetClosestMatchingEvent(e, noteEvents, animTime, songLength);
  if (optIt)
  {
    const auto it = *optIt;
 
    // Save the iterator - once we've graded the player's attempt of the
    //  event it points to, we shouldn't grade it again, so we ignore 
    //  repeats of the same iterator.
    if (it == m_prevAttempt)
    {
#ifdef GRADE_DEBUG
std::cout << " - ignoring this player event, already graded.\n";
#endif
      // Give the player some feedback, never just ignore, right?
      PlayWav("pop");
      return;
    }

    // The note event we think the player is attempting to match
    const NoteEvent& ne = *it;
    // Grade the time difference between player and note event ne
    const float MAX_ERROR = 0.5f; // Max acceptable time diff, TODO CONFIG
    auto grade = grader.FinalGrade(e, ne, animTime, songLength, MAX_ERROR);

    // Prevent multiple attempts at the same event: store the iterator
    //  so we can check above.
    // Only remember if note on, and a valid attempt.
    if (e.m_on && grade.m_type != Grade::TOO_QUICK)
    {
#ifdef GRADE_DEBUG
std::cout << "Storing event so you can't try again\n";
#endif
      m_prevAttempt = it;
    }

    if (e.m_on && e.m_note == ne.m_note)
    {
      // Note on event, pitch is correct
#ifdef GRADE_DEBUG
std::cout << "** Correct note! " << e.m_note << "\n";
#endif
      FeedbackBalloon(grade);
      IncreaseScore(grade);
    }
    else if (e.m_on && e.m_note != ne.m_note)
    {
      // Note on event, pitch is INCORRECT
#ifdef GRADE_DEBUG
std::cout << "** Incorrect note! You played: " << e.m_note << " should be: " << ne.m_note << "\n";
#endif
      PlayWav(WAV_INCORRECT);
      Assert(grade.m_type == Grade::BAD_NOTE);
      FeedbackBalloon(grade);
      DecreaseLife(grade); // TODO Life boosters when we reach checkpoints
    }
    else
    {
      // Note off event - the pitches must match. We grade on time.
      Assert(!e.m_on);
      Assert(e.m_note == ne.m_note);
      // The visual feedback is different: show note trail and increasing
      //  score while note is being played.
      //FeedbackBalloon(grade);
    }
  }
  else
  {
#ifdef GRADE_DEBUG
std::cout << ":((( Couldn't find a matching event to grade against!\n";
#endif
    // Now I think the most likely cause is a note up event for a
    //  bad note down.
  }
}

void GSHero::OnActive() 
{
  GSBase::OnActive();  

  m_roundIsOver = false;

  bool ok = TheGameRoundManager::Instance()->Load();
  if (!ok)
  {
std::cout << "CATASTROPHE! Failed to load game round .csv!!!\n";
  }

  InitGui();

  // Reset previous attempt pointer
  // (Points to most recently graded event, so we don't grade the same
  //  event multiple times)
  m_prevAttempt = m_scrollScore->GetNoteEvents().end();
  
  // Reset score
  m_playerScore.SetGuiElement(m_gui, "score-text", "score-text-anim-trigger");
  m_playerScore.Reset(0);

  // Reset life
  m_lifePercent.SetGuiElement(m_gui, "num-lives-text", "num-lives-text-anim-trigger");
  m_lifePercent.Reset(100);

  // Set song title. This could do with a bit more razzle dazzle.
  auto elem = GetElementByName(m_gui, "song-title");
  auto songTitleText = dynamic_cast<GuiText*>(elem);
  if (songTitleText)
  {
    songTitleText->SetText(GetGameRound().m_title);
  }

std::cout << "Paused...\n";

  // Start count in after a short pause, TODO do anims etc in the mean time
  TheMessageQueue::Instance()->Add(new FuncMsg(
    [](){ TheGSHero::Instance()->Start(); }, SecondsFromNow(2.f)));
}

void GSHero::InitGui()
{
  // Set palette for keyboard and score, before we load them in.
  RCPtr<Palette> palette = new Palette;

  File paletteFile;
  if (!paletteFile.OpenRead(GetGameRound().m_palette))
  {
    // TODO Report Error gracefully
    std::cout << "Failed to open palette file.\n";
    Assert(false);
  }

  if (!palette->Load(paletteFile))
  {
    // TODO Report Error gracefully
    std::cout << "Failed to load colours from  palette file.\n";
    Assert(false);
  }

  // Get pointer to the score..
  auto* elem = GetElementByName(m_gui, "the-score");
  if (!elem)
  {
    std::cout << "GUI Score element called \"the-score\" not found.\n";
    Assert(0); 
  }
 
  m_scrollScore = dynamic_cast<GuiScrollScore*>(elem);

  if (!m_scrollScore)
  {
    std::cout << "GUI Score element called \"the-score\" was found but it's the wrong type.\n";
    Assert(0); 
  }

  // Set callback for note events, so we can spot missed attempts
  m_scrollScore->SetNoteEventCallback(Amju::OnNoteEvent);

  // We need to do this before we load the music score, because that's when
  //  we set the colours of the glyphs.
  // If super hard mode, we don't colour the notes, but we DO still use the
  //  palette note colour for trails, right?
  m_scrollScore->SetPalette(palette);

std::cout << "Loading music score...\n";
  // Now we can load the music for this game round.
  if (!m_scrollScore->LoadMusicScore(GetGameRound().m_musicScore))
  {
    std::cout << "Failed to load music!!!\n";
    Assert(0); // TODO better error handling
  }

  // Find the animator parent too.
  elem = m_scrollScore->GetParent();
  Assert(elem);
  m_scoreAnim = dynamic_cast<GuiDecAnimation*>(elem);
  if (!m_scoreAnim)
  {
    std::cout << "Score does not have an animator parent.\n";
    Assert(0);
  }

  // Set the animation time from the score meta data
  auto songLength = m_scrollScore->GetSongLengthSeconds();
  if (songLength)
  {
std::cout << "Song length is: " << *songLength << "\n";
    m_scoreAnim->SetCycleTime(*songLength);
  }
  else
  {
    // This is bad  
    std::cout << "Failed to get song length. Does the BMP and BEAT meta data exist in the score?\n";
    Assert(false); // TODO We need a decent error reporter that doesn't crash the game
  }

  // Find the keyboard
  elem = GetElementByName(m_gui, "the-keyboard");
  if (!elem)
  {
    std::cout << "GUI keyboard element called \"the-keyboard\" not found.\n";
    Assert(0); 
  }
 
  m_keyboard = dynamic_cast<GuiMusicKb*>(elem);

  if (!m_keyboard)
  {
    std::cout << "GUI keyboard element called \"the-keyboard\" was found but it's the wrong type.\n";
    Assert(0); 
  }

  // If super hard mode, we don't colour the keys. TODO
  m_keyboard->SetPalette(palette);

  // Get animation controller for translating keyboard
  elem = GetElementByName(m_gui, "keyboard-anim");
  if (!elem)
  {
    std::cout << "GUI element called \"keyboard-anim\" not found.\n";
    Assert(0); 
  }
  m_keyboardAnim = dynamic_cast<GuiDecAnimation*>(elem);
  Assert(m_keyboardAnim);

  // The child of this animator is the translate decorator for the keyboard
  m_keyboardTranslate = dynamic_cast<GuiDecTranslate*>(m_keyboardAnim->GetChild());
  Assert(m_keyboardTranslate);

  // Hide GUI elements
  ShowFeedbackBalloon(false);

  // Pause animation until we finish count-in
  m_scoreAnim->SetIsPaused(true);
}

void GSHero::ShowFeedbackBalloon(bool showNotHide)
{
  auto elem = GetElementByName(m_gui, "feedback-balloon");
  if (elem)
  {
    elem->SetVisible(showNotHide);
  }
}

void GSHero::FeedbackBalloon(const Grade& g)
{
  Assert(g.m_type != Grade::UNGRADED);

  if (g.m_score > 0.5f)
  {
    PlayWav("good1");
  }
  else
  {
    PlayWav("rubber_ducky");
  }

  ShowFeedbackBalloon(true);

  auto elem = GetElementByName(m_gui, "feedback-text");
  if (elem)
  {
    auto t = dynamic_cast<GuiText*>(elem);
    if (t)
    {
      t->SetText(g.m_feedback);
    }
  }

  // Reset animation
  elem = GetElementByName(m_gui, "moving-anim");
  if (elem)
  {
    auto a = dynamic_cast<GuiDecAnimation*>(elem);
    if (a)
    {
      a->ResetAnimation();
    }
  }

  // TODO Set balloon colours according to score
}
}
