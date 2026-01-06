// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <iostream>
#include <sstream>
#include <CommandLineArgs.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include <SoundManager.h>
#include "Consts.h"
#include "Grader.h"
#include "GSHero.h"
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

void GSHero::Start()
{
std::cout << "START!!\n";

  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac

  // Callback for when count-in finished
  auto onFinished = []() { TheGSHero::Instance()->OnCountInFinished(); };

  // Play count in audio
  // TODO Get count in filename from game round data
  //TODO sm->PlaySong("Music/count-in.it");  

  // Start the count-in
  const int numCountInBeats = 4; // TODO Get from game round data
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
  // TODO Obvs this track comes from game round data
  sm->PlaySong("Music/amt1.it");
}

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
  GradeEvent(e);
}

void GSHero::GradeEvent(const MusicKbEvent& e)
{
std::cout << "Grading note event: Pitch: " << e.m_note 
  << " " << (e.m_on ? "*ON*" : "+off+")
  << "...\n";

  // TODO Play the note - player should always get audio feedback from the
  //  keyboard!
//  m_scrollScore->SendNoteEvent(e);

  float animTime = m_scrollScore->GetAnimTime();
  if (animTime == 0)
  {
    // We haven't started the action yet. 
    //PlayWav(WAV_INCORRECT); //?
    return;
  }

  auto optSongLength = m_scrollScore->GetSongLengthSeconds();
  // If we got here, we surely know the song length!
  if (!optSongLength)
  {
    std::cout << "*** Unexpected, can't get song length!!\n";
    return;
  }
  float songLength = *optSongLength;
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
    static auto prevIt = noteEvents.end();
    if (it == prevIt)
    {
std::cout << " - ignoring this player event, already graded.\n";
      return;
    }
    prevIt = it;

    const NoteEvent& ne = *it;
    if (e.m_on && e.m_note == ne.m_note)
    {
      // Note on event, pitch is correct
std::cout << "** Correct note! " << e.m_note << "\n";
      auto grade = grader.GradeTime(ne, animTime, songLength);
      FeedbackBalloon(grade);
    }
    else if (e.m_on && e.m_note != ne.m_note)
    {
      // Note on event, pitch is INCORRECT
std::cout << "** Incorrect note! You played: " << e.m_note << " should be: " << ne.m_note << "\n";
      PlayWav(WAV_INCORRECT);
    }
    else
    {
      // Note off event - the pitches must match. We grade on time.
      Assert(!e.m_on);
      Assert(e.m_note == ne.m_note);
      auto grade = grader.GradeTime(ne, animTime, songLength);
      FeedbackBalloon(grade);
    }
  }
  else
  {
std::cout << ":((( Couldn't find a matching event to grade against!\n";
    // This could be because we played a note when we shouldn't?
  }
}

void GSHero::OnActive() 
{
  GSBase::OnActive();  

  // Set palette for keyboard and score, before we load them in.
  RCPtr<Palette> palette = new Palette;

  File paletteFile;
  // TODO Filename should come from game round info
  if (!paletteFile.OpenRead("test_palette.txt"))
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
  m_scrollScore->SetPalette(palette);

std::cout << "Loading music score...\n";
  // Now we can load the music for this game round.
  // TODO music score filename also comes from game round info.
  if (!m_scrollScore->LoadMusicScore("test_hero_score.txt"))
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

  m_keyboard->SetPalette(palette);

  // Pause animation until we are ready to start, right?
  m_scoreAnim->SetIsPaused(true);

std::cout << "Paused...\n";

  // Start after a short pause, TODO TEMP TEST
  TheMessageQueue::Instance()->Add(new FuncMsg(
    [](){ TheGSHero::Instance()->Start(); }, SecondsFromNow(2.f)));

  // Hide GUI elements
  ShowFeedbackBalloon(false);
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
  ShowFeedbackBalloon(true);

  auto elem = GetElementByName(m_gui, "feedback-text");
  if (elem)
  {
    auto t = dynamic_cast<GuiText*>(elem);
    if (t)
    {
      std::stringstream ss;
      ss << g.m_score;
      t->SetText(ss.str());
    }
  }
}
}
