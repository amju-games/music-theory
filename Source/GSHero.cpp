// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <iostream>
#include <CommandLineArgs.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <SoundManager.h>
#include <Timer.h>
#include "BassPlayMidi.h"
#include "Consts.h"
#include "Grader.h"
#include "HeroGameRound.h"
#include "GSHero.h"
#include "GSHeroEnd.h"
#include "GSHeroWin.h"
#include "GSPause.h"
#include "Hud.h"
#include "PlayWav.h"
#include "UserProfile.h"
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
static int frameCount = 0;

static const HeroGameRound* s_gameRound = nullptr;

const HeroGameRound& GSHero::GetGameRound() const
{
  if (s_gameRound == nullptr)
  {
    s_gameRound = &TheGameRoundManager::Instance()->GetGameRound(0);
  }

  return *s_gameRound;
}

static void OnNoteEvent(const NoteEvent& ne)
{
  TheGSHero::Instance()->OnNoteEvent(ne);
}

static void OnPauseButton(PGuiElement)
{
  TheGSHero::Instance()->OnPauseGame();
}

static void OnKeyboardHasFinishedMoving(Animator*)
{
  TheGSHero::Instance()->OnKeyboardHasFinishedMoving();
}

GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

bool GSHero::OnKeyEvent(const KeyEvent& ke)
{
  // Debug cheat buttons
#ifdef _DEBUG
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR &&
     (std::tolower(ke.key) == 'w'))
  {
    OnPlayerHasWon();
    return true;
  }
#endif
 
  if (GSBase::OnKeyEvent(ke)) return true;
  return false;
}

void GSHero::SetGameRound(const HeroGameRound* gameRound)
{
  s_gameRound = gameRound;
}

void GSHero::SetUpForResume()
{
  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. TODO

std::cout << "*** Pause resume time: " << m_pauseResumeTime << "\n";

  const auto& beats = m_scrollScore->GetBeats();
  // Find the beat closest to m_pauseResumeTime:
  //  first, find the beat after m_pauseResumeTime
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
    Assert(it->m_time > m_pauseResumeTime);

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

    m_pauseResumeTime = it->m_time; // (why do this? )

std::cout << "DONE! Resuming at: bar: " << it->m_bar 
  << " beat: " << it->m_beat 
  << " time: " << it->m_time << "\n";

    // Set the resume point of the score 
    m_scrollScore->AnimateSpecial(m_pauseResumeTime, 0);
  }
}

void GSHero::ResumeGame()
{
  ChangeState(HeroState::COUNT_IN);

  auto& gameround = GetGameRound();

  //auto sm = TheSoundManager::Instance();

  // Preload main backing track
  // Not req for midi
  //sm->Preload(gameround.m_backingTrack);

  // Start playing the count-in track
  //sm->PlaySong(gameround.m_countIn);  
  PlayMidiSong(gameround.m_countIn);  

  ShowCountInGui();
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
  // Here we need to set the score position so we play the song from the
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
}

// TODO promote handy function
template <typename T> 
T Mean(T t1, T t2)
{
  return (t1 + t2) / T(2);
}

void GSHero::UpdateKeyboardPosition()
{
  // Set keyboard x-coord so that upcoming notes will be playable.

  // Look ahead to see what note events will be coming soon.
  // Get the min/max note pitches in the current section.
  const auto& noteEvents = m_scrollScore->GetNoteEvents();

  if (m_sectionIndex >= static_cast<int>(m_songSections.size()))
  {
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

  // Move the keyboard so all notes in the section are centred on screen. 
  // Although, centring looks good, it is bad for playability :(

  // Get the min and max notes in the range
  const auto optSectionMinMaxNotes = FindMinMaxPitchInSection(s, noteEvents);
  if (!optSectionMinMaxNotes) 
  {
std::cout << "ERROR: no min/max notes in song section " << m_sectionIndex << "\n";
    return;
  }
  const int sectionMin = optSectionMinMaxNotes->first;
  const int sectionMax = optSectionMinMaxNotes->second;
  if (   sectionMin == -1 || sectionMax == -1
      || sectionMin == 0  || sectionMax == 0) // non-note events in section
  {
std::cout << "ERROR: non-note events in song section " << m_sectionIndex << "\n";
    return;
  }
std::cout << "Section min note pitch: " << sectionMin 
  << " section max: " << sectionMax << "\n";

  Assert(m_keyboard->GetKey(sectionMin)); // bad section, contains non-notes?
  Assert(m_keyboard->GetKey(sectionMax)); // bad section, contains non-notes?

  // Get mid point in screen coords of notes in section
  float sectionMiddle = Mean(m_keyboard->GetKeyMidX(sectionMin), 
                             m_keyboard->GetKeyMidX(sectionMax));

  const int screenMin = m_keyboard->GetMinKeyOnScreen();
  const int screenMax = m_keyboard->GetMaxKeyOnScreen();
  if (screenMin == -1 || screenMax == -1)
  {
std::cout << "ERROR: keyboard: no min/max keys on screen?\n";
    return;
  }

  if (sectionMin > screenMin && sectionMax < screenMax) 
  {
    // Don't centre... it looks great but makes it harder to play.
    return; // no need to do anything, let the animation finish
  }

  // Get mid point in screen coords of notes currently on screen
  const float screenMiddle = Mean(m_keyboard->GetKeyMidX(screenMin),
                                  m_keyboard->GetKeyMidX(screenMax));

  // Try to get middle note of section in the middle of the screen
  const auto& currentPos = m_keyboardTranslate->GetLocalPos();
  auto desiredPos = currentPos;
  float distance = screenMiddle - sectionMiddle;
  desiredPos.x += distance;

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
  m_keyboardIsMoving = true;
  // Urgh, set and reset this callback, or it keeps getting called
  //  every frame! TODO Fix that, in Animator.
  m_keyboardAnim->SetOnCompleteCallback(Amju::OnKeyboardHasFinishedMoving);
}

void GSHero::OnKeyboardHasFinishedMoving()
{
  // Urgh, set and reset this callback, or it keeps getting called
  //  every frame! TODO Fix that, in Animator.
  m_keyboardAnim->SetOnCompleteCallback(nullptr);

  m_keyboardIsMoving = false;
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
  UpdateHud();

  // Scroll the score if we are playing the song.
  if (m_state == HeroState::SONG_PLAYING)
  {
    auto sm = TheSoundManager::Instance();
    float songElapsedSeconds = //sm->GetSongElapsedTimeSeconds();
      GetMidiSongElapsedTimeSeconds();
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

  // Update keyboard pos after state has initialised
  frameCount++;
  if (frameCount == 2)
  {
    UpdateKeyboardPosition();
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

// TODO This is no good, it should be time, not number of frames, surely?!
static const int NUM_UPDATE_NUM_FRAMES = 50;

void GSHero::IncreaseScore(const Grade& grade)
{
  int amount = static_cast<int>(std::round(grade.m_score * 1000.f));
  amount *= 100;

  GetHud().m_playerScore.Add(amount, NUM_UPDATE_NUM_FRAMES);

  GetHud().SetPatchSizes();
}

void GSHero::DecreaseLife(const Grade& grade)
{
  int dec = GetGameRound().m_lifeDecrease;
  dec = std::abs(dec);
  auto& life = GetHud().m_playerLife;
  life.Add(-dec, NUM_UPDATE_NUM_FRAMES); 

  if (life.m_internalNumber <= 0)
  {
    life.m_internalNumber = 0;
    OnPlayerHasLost();
  }
}

void GSHero::OnPlayerHasWon()
{
std::cout << "Player has won this round!\n";

  m_roundIsOver = true;
  m_pauseResumeTime = 0;
  ChangeState(HeroState::PLAYER_HAS_WON);

  // Save progress -- TODO is there a better approach? We don't want
  //  to lose the player progress if the process terminates.
  auto user = GetUserProfile();
  auto song = user->GetSongPlayerInfo(GetGameRound().m_name);
  if (song.m_completed == false) // first time completing the song?
  {
    song.m_completed = true;
    user->SetSongPlayerInfo(song);
    user->Save();
  }
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

#if defined(AMJU_USE_BASS_MIDI) && !defined(AMJU_IOS)
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
  //sm->Preload(gameround.m_backingTrack);

  // Start playing the count-in track
  PlayMidiSong(gameround.m_countIn);
  ShowCountInGui();

  // Callback for when count-in finished
  // Now we are doing this in Update old school.
  // TODO Remove this callback stuff.
  auto onFinished = []() { };

  // Start the count-in on the music score.
  const int numCountInBeats = gameround.m_numCountInBeats;

std::cout << "Count-in: there are " << numCountInBeats << " beats.\n";
  m_scrollScore->StartCountIn(numCountInBeats, onFinished);
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

  // Start playing the backing track for this song
  auto sm = TheSoundManager::Instance();
  PlayMidiSong(GetGameRound().m_backingTrack);
  MidiMutePlayerChannel(true);

  // Set start point of song: convert normalised time to seconds
  float seekPos = m_pauseResumeTime * m_scoreLengthSeconds;
  MidiSeek(seekPos); 

  // At this point, the count in has finished, so no more need for this?
  // Actually it prob doesn't matter, it will get overwritten as we 
  //  play forward from this point.
  //m_pauseResumeTime = 0;

  ResetMissedNoteCounters();

  // Hide count-in (should we go further and remove it from m_gui?)
  m_countInGui->SetVisible(false);
}

void GSHero::ShowCountInGui()
{
  m_countInGui = LoadGui(GetGameRound().m_countInGui, false);

  // Expect this to have a comp called "count-in-comp" with animator children. Each child
  //  shows a count-in beat/number, so we set times so these go off one at a time.
  auto countInGuiComp = dynamic_cast<GuiComposite*>(
    GetElementByName(m_countInGui, "count-in-comp"));
  Assert(countInGuiComp); // gui must follow expected format!

  int numBeats = countInGuiComp->GetNumChildren();
  float beatTime = m_countInExpiryTime / static_cast<float>(numBeats);

  // Set anim time for each count-in beat
  for (int i = 0; i < numBeats; ++i)
  {
    auto anim = dynamic_cast<GuiDecAnimation*>(countInGuiComp->GetChild(i));
    Assert(anim);
    anim->SetCycleTime(static_cast<float>(i) * beatTime);
  }

  // Add count-in gui to main gui for this state
  auto rootComp = dynamic_cast<GuiComposite*>(m_gui.GetPtr());
  Assert(rootComp);
  rootComp->AddChild(m_countInGui);
}

void GSHero::OnNoteEvent(const NoteEvent& ne)
{
  // This is a callback for a music/note event generated by the Score.
  
  // End of song? Ignore event.
  if (m_sectionIndex >= static_cast<int>(m_songSections.size()))
    return;

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
std::cout << "Music KB event: " 
  << e.m_note << " " 
  << (e.m_on? "on" : "off")
  << "\n";
#endif  // MUSIC_EVENT_DEBUG

  if (m_roundIsOver)
  {
    return;
  }

  if (m_keyboardIsMoving)
  {
#ifdef MUSIC_EVENT_DEBUG
std::cout << "Not grading event, keyboard is moving. (" 
  << e.m_note << " " 
  << (e.m_on? "on" : "off")
  << ")\n";
#endif  // MUSIC_EVENT_DEBUG

  }
  else
  {
    GradeEvent(e);
  }
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

  frameCount = 0;
  m_keyboardIsMoving = false;
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
  bool reset = (m_pauseResumeTime == 0);
  InitHud(m_gui, reset);
}

void GSHero::SetSongTitle()
{
  // Set song title. This could do with a bit more razzle dazzle.
  auto elem = GetElementByName(m_gui, "song-title");
  auto songTitleText = dynamic_cast<IGuiText*>(elem);
  if (songTitleText)
  {
    songTitleText->SetText(GetGameRound().m_title);
  }

  // Set Composer - also subtitle, TODO. Maybe there isn't space, and
  //  we just show subtitle when we choose piece.
  elem = GetElementByName(m_gui, "song-composer");
  auto songComposerText = dynamic_cast<IGuiText*>(elem);
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
  const auto& events = m_scrollScore->GetNoteEvents();
  m_songSections = FindSongSections(events);

  // Add sections added manually to the game round. This is necessary 
  //  if there are no identifiable sections, but can be a mixture.
  AddGameRoundSections(
    m_songSections, 
    GetGameRound().m_sectionEndBarNumbers, // list of bar numbers
    events, // all note (and rest) events 
    m_scrollScore->GetBeats()); // time for each bar

std::cout << "Here are the final sections:\n";
for (const auto& s : m_songSections)
{
  std::cout << s << "\n";
}
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
  GuiElement* elem = GetElementByName(m_gui, "quit-button");
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
  // Get rid of all events except for the type we are looking for.
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
    auto t = dynamic_cast<IGuiText*>(elem);
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
