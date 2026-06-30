// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <iostream>
#include "Resumer.h"

namespace Amju
{
// it points to a beat in the BeatVec for the piece.
// Decrement it until we point to the first beat in a bar.
// Set time to the time of the beat it points to.
void Resumer::GoToFirstBeatOfBar(
  const BeatVec& beats, BeatVec::const_iterator it, float& time)
{ 
  // Go back to beat 1 in the current bar
  while (it != beats.begin() && it->m_beat > 1) 
  { 
    --it; 
std::cout << "Decrementing.... at: bar: " << it->m_bar
  << " beat: " << it->m_beat
  << " time: " << it->m_time << "\n";
  } 
  
  time = it->m_time;
}   

// Look for a note event at the given time; return true if found.
// This is used to check if there is a note event at the time
//  of the first beat of the bar. If not, we can go back to the prev bar.
bool Resumer::NoteEventWasFoundAtTime(float time, const NoteEvents& noteEvents)
{
  // Find note event at the given time
  NoteEvent searchVal;
  searchVal.m_time = time;
  auto noteIt = std::lower_bound(noteEvents.begin(), noteEvents.end(), searchVal);
  if (noteIt == noteEvents.end())
  {
    std::cout << "**RESUME: no note event corresponding to resume time " << time << "\n";
    return true; // Just go with it
  }
      
  std::cout << "**RESUME: Note event for resume time " << time << " is: " << noteIt->ToString() << "\n";
  // Check if the note event is close enough to the resume time (account for 
  //  float precision)
  float diff = std::abs(noteIt->m_time - time);
  if (diff < 0.00001f) // say
  { 
    std::cout << "** This looks like the GOOD CASE.\n";
    return true;
  }
  
  std::cout << "** This looks like the BAD CASE.\n";
    
  if (noteIt != noteEvents.begin())
  {
    --noteIt;
    std::cout << "**RESUME: previous event is: " << noteIt->ToString() << "\n";
  } 
  return false;
}   

// Given the song time at which the game round was paused, find the
//  appropriate resume time. 
// To do this, we need info about the beats and bars in the song. We 
//  want to go back to the first beat in the current bar, or the first
//  beat of an earlier bar if the current bar isn't suitable. E.g. a 
//  tied note.
float Resumer::FindResumePoint(float pauseTime, const BeatVec& beats, const NoteEvents& cNoteEvents)
{
std::cout << "*** Pause time: " << pauseTime << "\n";

  float resumeTime = pauseTime;

  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. TODO

  //const auto& beats = m_scrollScore->GetBeats();

  // Find the beat closest to resumeTime:
  //  first, find the next beat after resumeTime
  auto it = std::upper_bound(
    beats.begin(), beats.end(), resumeTime,
   [] (float time, const auto& beat) { return time < beat.m_time; }
  );

  if (it == beats.end())
  {
    // This is weird, we must have reached the end of the song. So go to
    //  winner state, right?
    // TODO
std::cout << "VERY STRANGE, on resuming, we seem to be at the end of the song?\n";
    // Return a super high value for resume time, so GSHero will detect the
    //  end of the round.
    ///OnPlayerHasWon();
    return HUGELY_LONG_TIME;
  }

  Assert(it->m_time > resumeTime);

  // TODO Find a better way to do this: we are copying the vec and erasing.
  //NoteEvents noteEvents = m_scrollScore->GetNoteEvents();
  auto noteEvents(cNoteEvents);
  // Strip out everything except note on and rest on events
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
    [](const NoteEvent& ne)
    {  
      return !ne.IsNoteOnEvent() && !ne.IsRestOnEvent();
    }),
    noteEvents.end());

  // Go back until we are pointing to the first beat of a bar,
  //  and there is a note on or rest on event at that point.
  bool goodTimeFound = false;
  while (!goodTimeFound)
  {
    GoToFirstBeatOfBar(beats, it, resumeTime);

    if (NoteEventWasFoundAtTime(resumeTime, noteEvents))
    {
      // Good - we are finished
      goodTimeFound = true;
    }
    else if (it == beats.begin())
    {
      // At start of song?! - so we are done.
      return 0;

//      goodTimeFound = true;
//      resumeTime = 0; // we might as well restart
    }
    else
    {
      --it; // go back to prev beat, i.e. the last beat of the prev bar.
    }
  }

  return resumeTime;
}

}

