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
#include "GSPause.h"
#include "NumUpdate.h"
#include "PlayWav.h"
#include "UseVertexColourShader.h"

#ifdef WIN32
// Utterly criminal
#undef min
#undef max
#endif

//#define KEYBOARD_DEBUG
//#define MISSED_NOTE_DEBUG

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

static void OnPauseButton(PGuiElement)
{
  // TODO Notify via Game?
  TheGSHero::Instance()->OnPauseGame();
}

GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

void GSHero::SetUpForResume()
{
  // Do the things for a Resume we need to do immediately: calc
  //  and show the resume point in the score. 

  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. TODO

  // Convert pauseResumeTime into seconds
  auto optSongLength = m_scrollScore->GetSongLengthSeconds();
  float songLength = *optSongLength;
  float seconds = m_pauseResumeTime * songLength;

  // Immediately set the position of the score where we will restart from.
  // Restart the scrolling score from the resume point
  m_scrollScore->OnResetAnimation(); // wait, does this do anything?
  m_scoreAnim->SetAnimTimeSeconds(seconds);
  m_scoreAnim->SetIsPaused(false);
  m_scoreAnim->Update();
  m_scoreAnim->SetIsPaused(true);
}

void GSHero::ResumeGame()
{
  // Play the count-in track, then restart the anim and play backing
  //  track -- but set the seek position.
  // Restart the song from the resume point

  // Ideally, count-in before restarting the song and scroll anim.
  // Once the count-in is over, we can reset this resume time.
  // We can reset this now because there is no count-in on resume.
  m_pauseResumeTime = 0;

  // TODO TEMP TEST - un-pause the scroll anim, play track
  OnCountInFinished();
}

void GSHero::ResumeOrRestartGame()
{
  // HUD values are restored in ResetHUD.
  // Here we need to set the score position and play the song from the
  //  time we were at when we were paused.

  if (m_pauseResumeTime > 0)
  {
    SetUpForResume();

    std::cout << "Resuming game in 3 secs!\n";

    // Resume after a short pause, TODO do anims etc in the mean time
    TheMessageQueue::Instance()->Add(new FuncMsg(
      [](){ TheGSHero::Instance()->ResumeGame(); }, SecondsFromNow(3.f)));
  }
  else
  {
    // Nothing to resume -- restart everything.
std::cout << " ...(re)starting round after this short pause..\n";

    // Start count in after a short pause, TODO do anims etc in the mean time
    TheMessageQueue::Instance()->Add(new FuncMsg(
      [](){ TheGSHero::Instance()->RestartGame(); }, SecondsFromNow(1.f)));
  }
}

void GSHero::OnPauseGame()
{
  TheGSPause::Instance()->SetPrevState(this);

  auto sm = TheSoundManager::Instance();
  sm->StopSong();

  if (m_roundIsOver)
  {
    // We were done - allow the timed message in flight to take us go to 
    //  the appropriate next state, which will not need a pause menu, 
    //  as it's just GUI.
    return;
  }
 
  // We don't want timed messages messing up the flow! 
  TheMessageQueue::Instance()->Clear();

  // Pause scrolling score -- if it was moving
  m_scoreAnim->SetIsPaused(true);

  float animTime = m_scrollScore->GetAnimTime();
  if (animTime > 0)
  {
    // During or before the count-in, do nothing special, just restart
    //  the round when we re-enter this state.
    // But if we have started the song, store the point we go to.
    // Check if we are in the first bar - if so, we just restart the
    //  round, as we can't go back to the previous bar.
    // TODO

    m_pauseResumeTime = animTime;
  }

  GoTo<TheGSPause>();

  // Resume options:
  // If we were before count-in started, restart the state.
  // If we are counting-in from the start of the song, just restart
  //  the song - also just restart the state.
  // If we pause during the song, it would be nice to restart with a 
  //  new count-in into the bar where we were paused.
  // If we pause during the 'resume count-in', restart the resume count-in.
  // If we pause after the song has finished, or if we lost and the song
  //  has stopped, we should go to the next state.
  // ... so it sounds like we need a state enum, sigh, to keep track of
  //  the current state, so we know how to resume.

  // OK how about this: we DO go to a separate paused state. 
  // The only info we need to resume is the time into the song, right?
  // In OnActive, we restore the state.
  // The global message queue still fucks it up, so we have to use a 
  //  local message queue.
}

void GSHero::UpdateKeyboardPosition()
{
  // Set keyboard x-coord so that upcoming notes will be playable.

  // Look ahead to see what note events will be coming soon.
  auto noteOnEvents = m_scrollScore->GetNoteEvents();

  // Just consider note on events 
  noteOnEvents.erase(
    std::remove_if(noteOnEvents.begin(), noteOnEvents.end(),
    [](const NoteEvent& ne)
    {
      return !ne.IsNoteOnEvent();
    }),
    noteOnEvents.end());

  float animTime = m_scrollScore->GetAnimTime();

  // Get all notes which will occur after the current anim time.
  auto it = std::upper_bound(noteOnEvents.begin(), noteOnEvents.end(), animTime,
    [](float t, const NoteEvent& ne) { return t < ne.m_time; });

  if (it == noteOnEvents.end())
  {
    return; // no more notes?! 
  }
 
  // Look ahead this many events. 
  const int lookAhead = 7;
  // Reduce the range if we are near the end of the note events.
  auto end = std::next(it, 
    std::min<size_t>(lookAhead, std::distance(it, noteOnEvents.end())));

  // Not:
  // auto end = std::min(it + lookAhead, noteOnEvents.end());
  // Which triggers iterator check and crashes the program in MSVC.

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

void GSHero::Draw2d()
{
  if (m_gui)
  {   
    UseVertexColourShader();
    m_gui->Draw();
  }
}

void GSHero::Update()
{
  GSBase::Update();

  m_playerScore.Update();
  m_lifePercent.Update();

  auto pos = m_scrollScore->GetLocalPos();
  m_scoreExtras->SetLocalPos(pos);

  float animTime = m_scrollScore->GetAnimTime();
  if (!m_roundIsOver && animTime > 0.9999f)
  {
    OnPlayerHasWon();
  }
}

void GSHero::ReloadGui()
{
  auto sm = TheSoundManager::Instance();
  sm->StopSong();

  GSBase::ReloadGui();
}

// TODO This is no good, it should be time, not number of frames, surely?!
static const int NUM_UPDATE_NUM_FRAMES = 50;

void GSHero::IncreaseScore(const Grade& grade)
{
  int amount = static_cast<int>(std::round(grade.m_score * 1000.f));
  amount *= 100;

  m_playerScore.Add(amount, NUM_UPDATE_NUM_FRAMES);
}

void GSHero::DecreaseLife(const Grade& grade)
{
  m_lifePercent.Add(-1, NUM_UPDATE_NUM_FRAMES); // TODO Balance this; currently only losing one point so we can easily play through the whole game round.

  if (m_lifePercent.m_internalNumber <= 0)
  {
    OnPlayerHasLost();
  }
}

void GSHero::OnPlayerHasWon()
{
std::cout << "Player has won this round!\n";

  m_roundIsOver = true;

  TheMessageQueue::Instance()->Clear();

  // Go to next game state after short delay
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGSHeroWin>,
    SecondsFromNow(2.f)));
}

void GSHero::OnPlayerHasLost()
{
std::cout << "Player has lost this round!\n";

  m_roundIsOver = true;

  TheSoundManager::Instance()->StopSong();
  PlayWav("record_scratch");

  // Stop scrolling - TODO Grind to a halt, not immediate stop
  m_scoreAnim->SetIsPaused(true);

  TheMessageQueue::Instance()->Clear();

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

void GSHero::InitSound()
{
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
}

void GSHero::RestartGame()
{
std::cout << "Restarting game.\n";

  auto& gameround = GetGameRound();

  // Play count-in audio
  // The BPM has to match that of the song. We don't want a huge number of
  //  count-in audio files, so better to adjust BPM of the count-in song, to 
  //  match the main song BPM.
  auto sm = TheSoundManager::Instance();
  // Preload main backing track
  sm->Preload(gameround.m_backingTrack);
  sm->PlaySong(gameround.m_countIn);  

  // Callback for when count-in finished
  auto onFinished = []() { TheGSHero::Instance()->OnCountInFinished(); };

  // Start the count-in
  const int numCountInBeats = GetGameRound().m_numCountInBeats;

std::cout << "Count-in: there are " << numCountInBeats << " beats.\n";
  m_scrollScore->StartCountIn(numCountInBeats, onFinished);

  UpdateKeyboardPosition();
}

void GSHero::ResetMissedNoteCounters()
{
  m_numPlayerNotes = 0;
  m_numScoreNotes = 0;

#ifdef MISSED_NOTE_DEBUG
std::cout << "RESET counters:\n";
std::cout << "  Num player notes: " << m_numPlayerNotes 
  << " Num score notes: " << m_numScoreNotes << "\n";
#endif
}

void GSHero::OnCountInFinished()
{
std::cout << "Count in finished!\n";

  // Start score animating, and set backing track playing at the same time.
  // The score and the song have to be in perfect sync.
  m_scoreAnim->SetIsPaused(false);

  // Start playing the backing track for this song
  auto sm = TheSoundManager::Instance();
  sm->PlaySong(GetGameRound().m_backingTrack);

  ResetMissedNoteCounters();
}

void GSHero::OnNoteEvent(const NoteEvent& ne)
{
  // This is a callback for a music/note event generated by the Score.

  if (m_roundIsOver)
  {
    return;
  }

  if (ne.IsNoteOnEvent())
  {
    // Update the position of the keyboard, so that upcoming notes will
    //  be visible and playable. As an optimisation, we only do this
    //  update when a note event happens, rather than every frame.
    // Just update this once per note event, not on both note on and note off
    //  events. It doesn't really matter which event type we chose.
    UpdateKeyboardPosition();
  }

  // This is logic to detect a missed note, i.e. player does not 
  //  attempt.
  if (ne.IsNoteOffEvent())
  {
    // Note off: increment count of completed score-generated events
    m_numScoreNotes++;

#ifdef MISSED_NOTE_DEBUG
std::cout << "Score note! (" << ne.m_note << ", note off):\n";
std::cout << "  Num player notes: " << m_numPlayerNotes 
  << " Num score notes: " << m_numScoreNotes << "\n";
#endif
  }

  if (m_numScoreNotes > m_numPlayerNotes)
  {
#ifdef MISSED_NOTE_DEBUG
std::cout << "*** Player has missed a note, I think!!!\n";
std::cout << "  Num player notes: " << m_numPlayerNotes 
  << " Num score notes: " << m_numScoreNotes << "\n";
#endif

    m_numPlayerNotes = m_numScoreNotes;
#ifdef MISSED_NOTE_DEBUG
std::cout << "  ... Resetting: counters now equal:\n";
std::cout << "  Num player notes: " << m_numPlayerNotes 
  << " Num score notes: " << m_numScoreNotes << "\n";
#endif

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

      // This is the right place to increment player note count?
      m_numPlayerNotes++;
#ifdef MISSED_NOTE_DEBUG
std::cout << "Player note! (" << e.m_note << ", note down):\n";
std::cout << "  Num player notes: " << m_numPlayerNotes 
  << " Num score notes: " << m_numScoreNotes << "\n";
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

void GSHero::OnDeactive() 
{
  GSBase::OnDeactive();
  auto sm = TheSoundManager::Instance();
  sm->ClearPreloadedSongs(); 
}


void GSHero::OnActive() 
{
  GSBase::OnActive();  

  m_roundIsOver = false;

  bool ok = TheGameRoundManager::Instance()->Load();
  if (!ok)
  {
std::cout << "CATASTROPHE! Failed to load game round .csv!!!\n";
    Assert(0);
  }

  InitGui();
 
  // Set up but don't start playing anything yet
  InitSound();
 
  // Resume game if we have restarted after a pause.
  // If we weren't paused, just restart.
  ResumeOrRestartGame();
}

void GSHero::ResetHud()
{
  // Find GUI elements
  m_playerScore.SetGuiElement(m_gui, "score-text", "score-text-anim-trigger");
  m_lifePercent.SetGuiElement(
    m_gui, "num-lives-text", "num-lives-text-anim-trigger");

  // Reset score and life values: but only if we are not resuming 
  //  from being paused.
  if (m_pauseResumeTime == 0)
  {
    m_playerScore.Reset(0);
    m_lifePercent.Reset(100); 
  }
  else
  {
    m_playerScore.ResumeAfterPause();
    m_lifePercent.ResumeAfterPause();
  }
}

void GSHero::SetSongTitle()
{
  // Set song title. This could do with a bit more razzle dazzle.
  auto elem = GetElementByName(m_gui, "song-title");
  auto songTitleText = dynamic_cast<GuiText*>(elem);
  if (songTitleText)
  {
    songTitleText->SetText(GetGameRound().m_title);
  }

  // Set Composer - also subtitle, TODO. Maybe there isn't space, and
  //  we just show subtitle when we choose piece.
  elem = GetElementByName(m_gui, "song-composer");
  auto songComposerText = dynamic_cast<GuiText*>(elem);
  if (songComposerText)
  {
    songComposerText->SetText(GetGameRound().m_composer);
  }
  
}

RCPtr<Palette> GSHero::LoadPalette()
{
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

  return palette;
}

void GSHero::InitScrollScore()
{
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
}

void GSHero::LoadMusicScore()
{
  const auto& score = GetGameRound().m_musicScore;

std::cout << "Loading music score: " << score << "...\n";

  if (!m_scrollScore->LoadMusicScore(score))
  {
    std::cout << "Failed to load score: " << score << "\n";
    Assert(0); // TODO better error handling
  }
}

void GSHero::InitScrollScoreAnim()
{
  // Find the animator parent too.
  auto elem = GetElementByName(m_gui, "play-score");
  Assert(elem);
  m_scoreAnim = dynamic_cast<GuiDecAnimation*>(elem);
  if (!m_scoreAnim)
  {
    std::cout << "Score does not have an animator.\n";
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
}

void GSHero::InitKeyboard()
{
  // Find the keyboard
  auto elem = GetElementByName(m_gui, "the-keyboard");
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
}

void GSHero::InitPauseButton()
{
  GuiElement* elem = GetElementByName(m_gui, "pause-button");
  Assert(elem);
  elem->SetCommand(Amju::OnPauseButton);
}

void GSHero::InitGui()
{
  auto palette = LoadPalette();

  InitScrollScore();

  // We need to do this before we load the music score, because that's when
  //  we set the colours of the glyphs.
  // If super hard mode, we don't colour the notes, but we DO still use the
  //  palette note colour for trails, right?
  m_scrollScore->SetPalette(palette);

  LoadMusicScore();

  InitScrollScoreAnim();

  InitKeyboard();

  InitPauseButton();

  // Set keyboard palette
  // If super hard mode, we don't colour the keys. TODO
  m_keyboard->SetPalette(palette);

  // Hide GUI elements
  ShowFeedbackBalloon(false);

  // Pause animation until we finish count-in
  m_scoreAnim->SetIsPaused(true);

  SetSongTitle();

  ResetHud();

  // Reset previous attempt pointer
  // (Points to most recently graded event, so we don't grade the same
  //  event multiple times)
  m_prevAttempt = m_scrollScore->GetNoteEvents().end();

  AttachExtraBits(); 
}

void GSHero::AttachExtraBits()
{
  // TODO TEMP TEST 
  // Attach a heart to the score 
  auto heart = LoadGui("Gui/extra-heart.txt");
  AttachExtraBitToScore(heart, 1, NoteEventType::NOTE_ON);

  auto heart2 = LoadGui("Gui/extra-heart.txt");
  AttachExtraBitToScore(heart2, 2, NoteEventType::NOTE_ON);
}

void GSHero::AttachExtraBitToScore(
  PGuiElement extra, int eventNum, NoteEventType net)
{
  // TODO for now, we are only supporting note on events.

  auto elem = GetElementByName(m_gui, "score-extras");
  m_scoreExtras = dynamic_cast<GuiComposite*>(elem);
  if (!m_scoreExtras)
  {
std::cout << "Failed to find score-extras!\n";
    return;
  }

  m_scoreExtras->AddChild(extra);

  const auto& noteEvents = m_scrollScore->GetNoteEvents();
  // Get rid of note up events, so we just have note down events.
  // Probably best to just iterate over the events, counting the type
  //  we are interested in.
  auto notesCopy(noteEvents);
  notesCopy.erase(
    std::remove_if(notesCopy.begin(), notesCopy.end(), 
     [=](const NoteEvent& ne) { return ne.m_type != net; }),
    notesCopy.end());

  const auto& ne = notesCopy[eventNum];

  // Find the position of the note or rest so we can place the extra
  //  GUI on top -- extra's local pos then finesses the position.
  Vec2f pos = ne.GetPos();
  Vec2f scale = m_scrollScore->GetSize();
 
  pos *= scale; // or just scale x ?? TODO

  extra->SetLocalPos(pos + extra->GetLocalPos());
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
