// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <algorithm>
#include <iostream>
#include <CommandLineArgs.h>
#include <MessageQueue.h>
#include <SoundManager.h>
#include "Consts.h"
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

  // Start score scrolling, and set backing track playing at the same time.
  // The score and the song have to be in perfect sync.
  // TODO Is there a way to sync the song?
  m_scoreAnim->SetIsPaused(false);

  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac
  sm->PlaySong("Music/amt1.it");
}

void GSHero::OnMusicKbEvent(const MusicKbEvent& e) 
{
  GradeEvent(e);
}

static auto GetRangeToConsider(
  const MusicKbEvent& e, const NoteEvents& noteEvents, float animTime, float songLength)
{
  // Find note events before and after animTime. 
  // Creating a dummy NoteEvent to use as the search value for equal_range.
  //  (We want to search on animTime, which is not the same type
  //  as what's in the container. This is as good/bad as the alternatives :))
  NoteEvent searchVal; 
  searchVal.m_time = animTime;
  // Get iterators to elements with times >= and > animTime
  auto [beforeIt, afterIt] = std::equal_range(
    noteEvents.begin(), noteEvents.end(), searchVal);

  // Expand range of events to include those preceding and following.
  if (beforeIt != noteEvents.begin())
  {
    // Drop down to previous element, so we can get the time of the last
    // event before animTime. 
    beforeIt--;
    // Now get all the events with the same time.
    searchVal.m_time = beforeIt->m_time;
    beforeIt = std::lower_bound(noteEvents.begin(), beforeIt, searchVal); 
  }
  // Same idea, for the note events immediately after:
  if (afterIt != noteEvents.end())
  {
    afterIt++;
    if (afterIt != noteEvents.end())
    {
      // Include all events that are the next time up from animTime.
      searchVal.m_time = afterIt->m_time;
      afterIt = std::upper_bound(afterIt, noteEvents.end(), searchVal);
    }
  }

#ifdef CLOSEST_EVENT_DEBUG
std::cout << "KEY DOWN! Note: " << e.m_note << " Time now: " << animTime << ", note events considered:\n";
for (auto it = beforeIt; it != afterIt; ++it)
{
  std::cout << " Time: " << it->m_time 
    << (it->m_onNotOff ? " ON " : " OFF ")
    << " Pitch: " << it->m_note
    << " Time diff: " << songLength * std::abs(animTime - it->m_time) << "s"
    << "\n";
}
#endif

  return std::make_pair(beforeIt, afterIt);
}

static std::optional<NoteEvents::const_iterator> FindBestMatch(
  const NoteEvents::const_iterator beforeIt, 
  const NoteEvents::const_iterator afterIt,
  const MusicKbEvent& e, float animTime, float songLength)
{
  // Find the most closely matching event: the same type, with the shortest
  //  time difference.
  // If this is a note off event, we're looking for the event with note off,
  //  same pitch as the player event, and closest time.
  bool found = false;
  float bestDiff = 1.f;
  auto bestIt = beforeIt;
  for (auto it = beforeIt; it != afterIt; ++it)
  {
    float diff = std::abs(animTime - it->m_time);

    if (   it->m_onNotOff == e.m_on  // event types match?
        && diff < bestDiff // closer time?
        && (e.m_on || it->m_note == e.m_note)) // if note off, pitches match?
    {
      bestDiff = diff;
      bestIt = it;
      found = true;
    }
  }

//#ifdef CLOSEST_EVENT_DEBUG
std::cout << "Best match:\n";
  std::cout << " Time: " << bestIt->m_time 
    << (bestIt->m_onNotOff ? " ON " : " OFF ")
    << " Pitch: " << bestIt->m_note
    << " Time diff: " << songLength * std::abs(animTime - bestIt->m_time) << "s"
    << "\n";
//#endif

  if (!found)
  {
    return std::nullopt;
  }

  return bestIt;
}

static std::optional<NoteEvents::const_iterator> GetClosestMatchingEvent(
  const MusicKbEvent& e, const NoteEvents& noteEvents, float animTime, float songLength)
{
  if (noteEvents.empty())
  {
    // There are no note events! Hmm, not sure what this would mean.
std::cout << "No note events, so don't know what to do!\n";
    return std::nullopt;
  }

  const auto [beforeIt, afterIt] = GetRangeToConsider(e, noteEvents, animTime, songLength);

  return FindBestMatch(beforeIt, afterIt, e, animTime, songLength);
}

static void GradeTime(const NoteEvent& ne, float animTime, float songLength)
{
  float timeDiff = animTime - ne.m_time; 
  float timeDiffSecs = timeDiff * songLength;

  const float PERFECT = 0.05f; // TODO TEMP TEST make these configurable
std::cout << 
  (std::abs(timeDiffSecs) < PERFECT ? " - *PERFECT TIMING*!" :
  (timeDiff < 0 ? " -- Too quick! " : " -- Too slow! "))
  << timeDiffSecs << "seconds ";

  const float MAX_ERROR = 0.25f; // TODO TEMP TEST
  if (std::abs(timeDiffSecs) > MAX_ERROR)
  {
std::cout << "NO POINTS!\n";
  }
  else
  {
    float grade = (1.f - std::abs(timeDiffSecs) / MAX_ERROR) * 100.f;
std::cout << " -- Grade: " << std::round(grade) << "%\n";
  } 
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
  auto optIt = GetClosestMatchingEvent(e, noteEvents, animTime, songLength);
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
      GradeTime(ne, animTime, songLength);
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
      GradeTime(ne, animTime, songLength);
    }
  }
  else
  {
std::cout << ":((( Couldn't find a matching event to grade against!\n";
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
}
}
