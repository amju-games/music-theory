// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <iostream>
#include <CommandLineArgs.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <SoundManager.h>
#include <Timer.h>
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
static const HeroGameRound& GetGameRound()
{
  auto& round = TheGameRoundManager::Instance()->GetGameRound(0);
  return round;
}

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
  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. TODO

std::cout << "*** Pause resume time: " << m_pauseResumeTime << "\n";

  const auto& beats = m_scrollScore->GetBeats();
  // Find the beat closest to m_pauseResumeTime:
  // First, find the beat after m_pauseResumeTime
  auto it = std::upper_bound(
    beats.begin(), beats.end(), m_pauseResumeTime,
   [] (float time, const auto& beat) { return time < beat.m_time; }
  );

  if (it == beats.end())
  {
    // This is weird, we must have reached the end of the song. So go to
    //  winner state, right?
    // TODO
std::cout << "VERY STRANGE, on resuming, we seem to be at the end of the song?\n";
  }
  else
  {
    // We have found the next beat after the pause resume time.
    // Go back to the prev beat.
    if (it != beats.begin()) --it;

std::cout << "Pause hit at: bar: " << it->m_bar 
  << " beat: " << it->m_beat 
  << " time: " << it->m_time << "\n";

    // Go back to beat 1 in the current bar
    while (it != beats.begin() && it->m_beat > 1) 
    {
      --it; 
std::cout << "Decrementing.... at: bar: " << it->m_bar 
  << " beat: " << it->m_beat 
  << " time: " << it->m_time << "\n";
    }

    // This time is correct, but we need to go to the next one
    //  to correctly position the score. I don't know why.
    m_pauseResumeTime = it->m_time;

    float scoreResumeTime = (++it)->m_time;

std::cout << "DONE! Resuming at: bar: " << it->m_bar 
  << " beat: " << it->m_beat 
  << " time: " << it->m_time << "\n";

    // Set the resume point of the score 
    m_scrollScore->AnimateSpecial(scoreResumeTime, 0);
  
    // Hopefully this will update the kb with the upcoming notes now
    //  we have set the resume point
    UpdateKeyboardPosition();
  }
}

void GSHero::ResumeGame()
{
  ChangeState(HeroState::COUNT_IN);

  auto& gameround = GetGameRound();

  auto sm = TheSoundManager::Instance();

  // Preload main backing track
  sm->Preload(gameround.m_backingTrack);

  // Start playing the count-in track
  sm->PlaySong(gameround.m_countIn);  

  // We can't use the count-in feature of GuiMusicScore, because we
  //  are in the middle of the song somewhere.
  // We will have to call OnCountInFinished using a timer -- or add a 
  //  callback to the count-in audio track.
  //OnCountInFinished()$

}

void GSHero::CancelResumeTime()
{
  // Called when player quits from pause menu, so when we re-enter
  //  Hero Mode, we restart the song from the beginning.
  m_pauseResumeTime = 0;
}

void GSHero::ResumeOrRestartGame()
{
  // HUD values are restored in ResetHUD.
  // Here we need to set the score position and play the song from the
  //  time we were at when we were paused.

  m_scrollScore->OnResetAnimation(); 

  const auto& gameround = GetGameRound();
  m_countInExpiryTime = gameround.m_numCountInBeats / m_scrollScore->GetBpm() * 60.f;
std::cout << "Count in time: " << m_countInExpiryTime << "\n";

  if (m_pauseResumeTime > 0)
  {
    SetUpForResume();
    ResumeGame();
  }
  else
  {
    // Nothing to resume -- restart everything.
    RestartGame();
  }
}

void GSHero::OnPauseGame()
{
  TheGSPause::Instance()->SetPrevState(this);

  auto sm = TheSoundManager::Instance();
  sm->StopSong();

  if (m_roundIsOver)
  {
    // We were done - go immediately to
    //  the appropriate next state, which will not need a pause menu, 
    //  as it's just GUI.
    if (m_state == HeroState::PLAYER_HAS_WON) 
    {
      GoTo<TheGSHeroWin>();
    }
    else if (m_state == HeroState::PLAYER_HAS_LOST)
    {
      GoTo<TheGSHeroEnd>();
    }
    // Reset state, riight?
    ChangeState(HeroState::NEW);
    return;
  }
 
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

// TODO const kb
static float GetOneWhiteKeyWidthScreenSpace(GuiMusicKbBase* keyboard)
{
//  return 0.15f;
  return keyboard->GetKey(60)->m_projectedRect.GetSize().x;
}

void GSHero::UpdateKeyboardPosition()
{
  // Set keyboard x-coord so that upcoming notes will be playable.

  // Look ahead to see what note events will be coming soon.
  // Get the min/max note pitches in the current section.
  const auto& noteEvents = m_scrollScore->GetNoteEvents();

  if (m_sectionIndex >= m_songSections.size()) 
  {
std::cout << "ERROR: Section index is " << m_sectionIndex << " but number of sections is: " << m_songSections.size() << "\n";

    return;
  }
  const Section& s = m_songSections[m_sectionIndex];

  // Colourise keyboard: Find all the notes we play in the section.  
  std::vector<int> keys;
  for (int i = s.first; i < s.second; ++i)
  {
    int note = noteEvents[i].m_note;
    if (note != -1)
    {
      keys.push_back(note);
    }
  }
  m_keyboard->ColouriseKeys(keys);

  // Move the keyboard so all notes in the section are on screen. 
  // Get the min and max notes in the range
  const auto optMinMaxNotes = FindMinMaxPitchInSection(s, noteEvents);
  if (!optMinMaxNotes) 
  {
std::cout << "ERROR: no min/max notes in song section " << m_sectionIndex << "\n";
    return;
  }
  const int minNote = optMinMaxNotes->first;
  const int maxNote = optMinMaxNotes->second;

#ifdef KEYBOARD_DEBUG
std::cout << "** Look ahead: min note: " << minNote << " max note: " << maxNote << "\n";
#endif

  // Convert distance in midi note values to screen space distance in x:
  // We use the screen space width of a white key. Count how many
  //  white keys we need to move, and mult by width.

  // Just get key width once: we won't be changing this, right??
  const float keyWidth = 
    GetOneWhiteKeyWidthScreenSpace(m_keyboard);

  if (keyWidth == 0)
  {
std::cout << "ERROR: key width is zero.\n";
  }

#ifdef KEYBOARD_DEBUG
std::cout << "Key width: " << keyWidth << "\n";
#endif

  const auto& currentPos = m_keyboardTranslate->GetLocalPos();
  auto desiredPos = currentPos;
  const int screenMin = m_keyboard->GetMinKeyOnScreen();
  const int screenMax = m_keyboard->GetMaxKeyOnScreen();
#ifdef KEYBOARD_DEBUG
std::cout << "  KB: Max on screen: " << screenMax  
  << " min on screen: " << screenMin
  << "\n";
#endif

  if (maxNote >= screenMax)
  {
    int whiteNotes = CountWhiteNotes(screenMax, maxNote);
    desiredPos.x -= keyWidth * static_cast<float>(whiteNotes);

#ifdef KEYBOARD_DEBUG
std::cout << "  Scroll L: Max on screen: " << screenMax 
  << " max coming up: " << maxNote 
//  << " white notes: " << whiteNotes 
  << " Desired X: " << desiredPos.x 
  << " Current X: " << currentPos.x 
  << "\n";
#endif
  }
  if (minNote <= screenMin) // both conditions should not be true!
  {
    int whiteNotes = CountWhiteNotes(minNote, screenMin);
    desiredPos.x += keyWidth * static_cast<float>(whiteNotes);

#ifdef KEYBOARD_DEBUG
std::cout << "  Scroll R: Min on screen: " << screenMin
  << " min coming up: " << minNote 
//  << " white notes: " << whiteNotes 
  << " Desired X: " << desiredPos.x 
  << " Current X: " << currentPos.x 
  << "\n";
#endif
  }
 
  if (minNote > screenMin && maxNote < screenMax) 
  {
    return; // no need to do anything, let the animation finish
  }

  // Copy current value to initial value, and set new desired value.
  // (Copying current to initial sounds like a useful thing to add 
  //  to the GuiDecorator class.)
  // Set current pos second, because GuiDecTranslate sets its value to 
  //  the last 'endpoint' set. We want to start the anim at currentPos,
  //  so this avoids a one-frame flicker as the current pos is adjusted.

  m_keyboardTranslate->SetTranslation(desiredPos, 1); 
  // ..sets current translation to desired pos

  m_keyboardTranslate->SetTranslation(currentPos, 0); 
  // ..sets current translation back to current pos

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

void GSHero::ChangeState(HeroState newState)
{
  m_timeInHeroState = 0;
  m_state = newState;
}

void GSHero::Update()
{
  GSBase::Update();

  m_playerScore.Update();
  m_lifePercent.Update();

  // Scroll the score if we are playing the song.
  if (m_state == HeroState::SONG_PLAYING)
  {
    auto sm = TheSoundManager::Instance();
    float songElapsedSeconds = sm->GetSongElapsedTimeSeconds();
    float normalisedAnimTime = songElapsedSeconds / m_scoreLengthSeconds;

    // Get 'dt' for animTime
    float dAnimTime = (m_prevAnimTime == 0 ? 0 :
      normalisedAnimTime - m_prevAnimTime);
    m_prevAnimTime = normalisedAnimTime;

    // Scroll the score.
    m_scrollScore->AnimateSpecial(normalisedAnimTime, dAnimTime);
  
    // Scroll the extras along with the score.
    auto pos = m_scrollScore->GetLocalPos();
    m_scoreExtras->SetLocalPos(pos);

    // If we have reached the end, we have won!
    if (!m_roundIsOver && normalisedAnimTime > 0.999f)
    {
      OnPlayerHasWon();
    }
  }

  // Update the time spent in the current 'micro state'
  m_timeInHeroState += TheTimer::Instance()->GetDt();

  // Check if we should change state -- we are not using timed messages,
  //  there are too many edge cases to worry about.
  // TODO config
  if (m_state == HeroState::PLAYER_HAS_WON && m_timeInHeroState > 3.f) 
  {
    GoTo<TheGSHeroWin>();
  }
  else if (m_state == HeroState::PLAYER_HAS_LOST && m_timeInHeroState > 3.f)
  {
    GoTo<TheGSHeroEnd>();
  }
  else if (m_state == HeroState::COUNT_IN && 
           m_timeInHeroState >= m_countInExpiryTime)
  {
    OnCountInFinished();
  }
}

void GSHero::ReloadGui()
{
  auto sm = TheSoundManager::Instance();
  sm->StopSong();

  GSBase::ReloadGui();
}

static int GetNumDigits(int s)
{
  int res = (s == 0 ? 1 : 0);
  while (s > 0)
  {
    s /= 10;
    ++res;
  }
  return res;
}

// TODO This is no good, it should be time, not number of frames, surely?!
static const int NUM_UPDATE_NUM_FRAMES = 50;

void GSHero::IncreaseScore(const Grade& grade)
{
  int amount = static_cast<int>(std::round(grade.m_score * 1000.f));
  amount *= 100;

  m_playerScore.Add(amount, NUM_UPDATE_NUM_FRAMES);

  auto size = m_playerScoreBg->GetSize();
  size.x = 0.08f + 0.07f * GetNumDigits(m_playerScore.m_internalNumber);
  m_playerScoreBg->SetSize(size);
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
  m_pauseResumeTime = 0;
  ChangeState(HeroState::PLAYER_HAS_WON);
}

void GSHero::OnPlayerHasLost()
{
std::cout << "Player has lost this round!\n";

  m_roundIsOver = true;
  m_pauseResumeTime = 0;
  ChangeState(HeroState::PLAYER_HAS_LOST);

  TheSoundManager::Instance()->StopSong();
  PlayWav("record_scratch");
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
std::cout << "Restarting game, here comes the count-in...\n";

  ChangeState(HeroState::COUNT_IN);

  auto& gameround = GetGameRound();

  // Play count-in audio
  auto sm = TheSoundManager::Instance();
  // Preload main backing track
  sm->Preload(gameround.m_backingTrack);

  // Start playing the count-in track
  sm->PlaySong(gameround.m_countIn);  

  // Callback for when count-in finished
  // Now we are doing this in Update old school.
  // TODO Remove this callback stuff.
  auto onFinished = []() { };

  // Start the count-in on the music score.
  const int numCountInBeats = gameround.m_numCountInBeats;

std::cout << "Count-in: there are " << numCountInBeats << " beats.\n";
  m_scrollScore->StartCountIn(numCountInBeats, onFinished);

  // This should bring the keyboard into the correct pos for the first
  //  section, but because the key width isn't set yet, it doesn't work.
  // TODO fix so this works.
  //UpdateKeyboardPosition();
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

  ChangeState(HeroState::SONG_PLAYING);

  // TODO we should already be in position by now, remove this once fixed.
  UpdateKeyboardPosition();

  // Start playing the backing track for this song
  auto sm = TheSoundManager::Instance();
  sm->PlaySong(GetGameRound().m_backingTrack);

  // Set start point of song: convert normalised time to seconds
  float seekPos = m_pauseResumeTime * m_scoreLengthSeconds;
  sm->SetSongSeekPosition(seekPos); 

  // At this point, the count in has finished, so no more need for this?
  // Actually it prob doesn't matter, it will get overwritten as we 
  //  play forward from this point.
  //m_pauseResumeTime = 0;

  ResetMissedNoteCounters();
}

void GSHero::OnNoteEvent(const NoteEvent& ne)
{
  // This is a callback for a music/note event generated by the Score.

  // Update current section 
  if (ne.GetId() >= m_songSections[m_sectionIndex].second)
  {
    ++m_sectionIndex;
std::cout << "New song section! " << m_sectionIndex << "\n";
    UpdateKeyboardPosition();
  }

  if (ne.m_time < m_pauseResumeTime)
  {
    // This happens when resuming: the score sends all note events up to the
    //  pause time.
    // But use the IDs we get to update to the current section, see above.
    return;
  }

  if (m_roundIsOver)
  {
    return;
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
    // Show the missed note TODO
    DecreaseLife(grade); 
  }
}

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
  // This is a player-generated event

#ifdef MUSIC_EVENT_DEBUG
std::cout << "Music KB event: " << e.m_note << " " 
  << (e.m_on? "on" : "off")
  << "\n";
#endif  // MUSIC_EVENT_DEBUG

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
    animTime = - timeBeforeSecs / m_scoreLengthSeconds;
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
  auto optIt = grader.GetClosestMatchingEvent(
    e, noteEvents, animTime, m_scoreLengthSeconds);
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
    auto grade = grader.FinalGrade(
      e, ne, animTime, m_scoreLengthSeconds, MAX_ERROR);

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
      // Not sure if we should play wav every time
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
  ChangeState(HeroState::NEW); 
  // Or should we use the state when hero mode was last active??

  // Start at song section 0: but if we are resuming, use all the note
  //  events we recv to move to the current section.
  m_sectionIndex = 0;

  m_prevAnimTime = 0; // always reset, right? What about resuming?

  // Can we just do this once?! Yes, but if we reload we get updates to the file.
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
  // Find and store pointers to GUI elements
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

  m_playerScoreBg = dynamic_cast<GuiPatch*>(GetElementByName(m_gui, "score-bg-patch"));
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

  // TODO Fixed palette across songs but can be changed for accessibility
  if (!palette->Load(GetGameRound().m_palette))
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

  // Set callback for note events generated by the score
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

  // Identify sections (phrases?) between which it is safe to move
  //  the keyboard (i.e. won't be a bad experience for the player).
  m_songSections = FindSongSections(m_scrollScore->GetNoteEvents());
}

void GSHero::InitScrollScoreAnim()
{
  // Set the animation time from the score meta data
  auto songLength = m_scrollScore->GetSongLengthSeconds();
  if (songLength)
  {
std::cout << "Song length is: " << *songLength << "\n";
    m_scoreLengthSeconds = *songLength;
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
 
  m_keyboard = dynamic_cast<GuiMusicKbBase*>(elem);

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
    // Sounds terrible //PlayWav("good1");
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
