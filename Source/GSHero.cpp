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
#include "NumUpdate.h"
#include "PlayWav.h"

namespace Amju
{
GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

void GSHero::Update()
{
  GSBase::Update();
}

void GSHero::ReloadGui()
{
  GSBase::ReloadGui();

  // Restart song from count in. It would be nice to start immediately,
  // e.g.
  // TheMessageQueue::Instance()->Clear(); // <- no such function exists :(
  // Start();
  auto sm = TheSoundManager::Instance();
  sm->StopSong();
}

void GSHero::IncreaseScore(const Grade& grade)
{
  int amount = std::round(grade.m_score * 1000.f);
  amount *= 100;
  m_playerScore += amount;
  NumUpdate(m_gui, "score-text", m_playerScore);
}

void GSHero::DecreaseLife(const Grade& grade)
{
  m_lifePercent -= 5;
  NumUpdate(m_gui, "num-lives-text", std::to_string(m_lifePercent) + "%");
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

  // Callback for when count-in finished
  auto onFinished = []() { TheGSHero::Instance()->OnCountInFinished(); };

  // Play count-in audio
  // The BPM has to match that of the song. We don't want a huge number of
  //  count-in audio files, so better to adjust BPM of the count-in song, to 
  //  match the main song BPM.
  sm->PlaySong(GetGameRound().m_countIn);  

  // Start the count-in
  const int numCountInBeats = GetGameRound().m_numCountInBeats;
  m_scrollScore->StartCountIn(numCountInBeats, onFinished);
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

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
  GradeEvent(e);
}

void GSHero::GradeEvent(const MusicKbEvent& e)
{
std::cout << "=================================\nGrading note event: Pitch: " << e.m_note 
  << " " << (e.m_on ? "*ON*" : "+off+");
  // No newline!

  // Get song length
  auto optSongLength = m_scrollScore->GetSongLengthSeconds();
  // If we got here, we surely know the song length!
  if (!optSongLength)
  {
std::cout << "\n";
    std::cout << "*** Unexpected, can't get song length!!\n";
    return;
  }
  float songLength = *optSongLength;

  float animTime = m_scrollScore->GetAnimTime();
  if (animTime >= 1.f)
  {
std::cout << "\n";
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
std::cout << "  * not even counting in yet bruv!\n";
      return;
    }
std::cout << " -- grade count-in event!\n";
    // Convert time in seconds to normalised time
    animTime = - timeBeforeSecs / songLength;
std::cout << "  - time before start: " << timeBeforeSecs << " normalised: " << animTime << "\n";
  }

std::cout << " AnimTime now: " << animTime 
  << "...\n";

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
std::cout << " - ignoring this player event, already graded.\n";
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
std::cout << "Storing event so you can't try again\n";
      m_prevAttempt = it;
    }

    if (e.m_on && e.m_note == ne.m_note)
    {
      // Note on event, pitch is correct
std::cout << "** Correct note! " << e.m_note << "\n";
      FeedbackBalloon(grade);
    }
    else if (e.m_on && e.m_note != ne.m_note)
    {
      // Note on event, pitch is INCORRECT
std::cout << "** Incorrect note! You played: " << e.m_note << " should be: " << ne.m_note << "\n";
      PlayWav(WAV_INCORRECT);
      Assert(grade.m_type == Grade::BAD_NOTE);
      FeedbackBalloon(grade);
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
std::cout << ":((( Couldn't find a matching event to grade against!\n";
    // Now I think the most likely cause is a note up event for a
    //  bad note down.
  }
}

void GSHero::OnActive() 
{
  GSBase::OnActive();  

  bool ok = TheGameRoundManager::Instance()->Load();
  if (!ok)
  {
std::cout << "CATASTROPHE! Failed to load game round .csv!!!\n";
  }

  InitGui();

  // Reset previous attempt pointer
  m_prevAttempt = m_scrollScore->GetNoteEvents().end();
  
  // Reset score
  m_playerScore = 0;
  NumUpdate(m_gui, "score-text", 0);

  // Reset life
  m_lifePercent = 100;
  NumUpdate(m_gui, "num-lives-text", "100%");

std::cout << "Paused...\n";

  // Start count in after a short pause, TODO TEMP TEST
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
