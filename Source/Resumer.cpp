// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <iostream>
#include "Resumer.h"

namespace Amju
{
// iter points to a beat in the BeatVec for the piece.
// Decrement iter until we point to the first beat in a bar.
// Set time to the time of the beat iter points to.
// NB we decrement the iterator in the calling code - note pass by ref.
float Resumer::GoToFirstBeatOfBar(
  const BeatVec& beats, BeatVec::const_iterator& iter)
{ 
  if (iter == beats.end()) 
  {
std::cout << "Unexpected, iter is at end of beats.\n";
    return 0;
  }

  // Go back to beat 1 in the current bar
  while (iter != beats.begin() && iter->m_beat > 1) 
  { 
    --iter; 
/*
std::cout << "Decrementing.... at: bar: " << iter->m_bar
  << " beat: " << iter->m_beat
  << " time: " << iter->m_time << "\n";
*/
  } 
 
  return iter->m_time;
}   

static bool TimesApproxEqual(float time1, float time2)
{
  static const float EPSILON = 0.00001f;
  return std::abs(time1 - time2) < EPSILON;
}

// Look for a note event at the given time; return true if found.
// This is used to check if there is a note event at the time
//  of the first beat of the bar. If not, we can go back to the prev bar.
bool Resumer::FindNoteEventAtTime(float time, const NoteEvents& noteEvents)
{
  // Search for note event at the given time: return true if found.

  NoteEvent searchVal; // create a NoteEvent to use as the value we want to find.
  searchVal.m_time = time; // this is easier than the alternatives.

  auto noteIt = std::lower_bound(noteEvents.begin(), noteEvents.end(), searchVal);
  if (noteIt == noteEvents.end())
  {
    std::cout << "**RESUME: no note event corresponding to resume time " << time << "\n";
    return false; // ??????? true; // Just go with it
  }
      
  std::cout << "**RESUME: Note event for resume time " << time << " is: " << noteIt->ToString() << "\n";
  // Check if the note event is close enough to the resume time (account for 
  //  float precision)
  if (TimesApproxEqual(noteIt->m_time, time))
  { 
    std::cout << "** This looks like the GOOD CASE.\n";
    return true;
  }
  
  std::cout << "** This looks like the BAD CASE.\n";

  // The search failed. But it could be because the matching event has a slightly
  //  lower start time, due to float precision. So check the previous event.
  if (noteIt != noteEvents.begin())
  {
    --noteIt;
    if (TimesApproxEqual(noteIt->m_time, time))
    {
      std::cout << "**RESUME: but previous event is: " << noteIt->ToString() << " so it's good after all!!\n";
      return true;
    }
  } 

  return false;
}   

// Find next beat following the given time.
BeatVec::const_iterator Resumer::NextBeatAfterTime(
  const BeatVec& beats, float resumeTime)
{
  auto iter = std::upper_bound(
    beats.begin(), beats.end(), resumeTime,
   [] (float time, const auto& beat) { return time < beat.m_time; }
  );
  return iter;
}

// Given the song time at which the game round was paused, find the
//  appropriate resume time. 
// To do this, we need info about the beats and bars in the song. We 
//  want to go back to the first beat in the current bar, or the first
//  beat of an earlier bar if the current bar isn't suitable. E.g. a 
//  tied note.
float Resumer::FindResumePoint(
  float pauseTime, const BeatVec& beats, const NoteEvents& cNoteEvents)
{
std::cout << "*** FIND RESUME POINT. Pause time: " << pauseTime << "\n";

  float resumeTime = pauseTime;

  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. TODO

  // Find the beat closest to resumeTime:
  // 1. Find the next beat _after_ resumeTime
  auto iter = NextBeatAfterTime(beats, resumeTime);

  if (iter == beats.end())
  {
    // We must have reached the end of the song. 
std::cout << "VERY STRANGE, on resuming, we seem to be at the end of the song?\n";
    // Return a super high value for resume time, so GSHero will detect the
    //  end of the round.
    return HUGELY_LONG_TIME;
  }

  // We have got the next beat after resumeTime.
  Assert(iter->m_time > resumeTime);

  // When we search noteEvents, we only care about note and rest ON events.
  // TODO Find a better way to do this: we are copying the vec and erasing.
  auto noteEvents(cNoteEvents);
  // Strip out everything except note on and rest on events
  noteEvents.erase(
    std::remove_if(noteEvents.begin(), noteEvents.end(),
    [](const NoteEvent& ne)
    {  
      return !ne.IsNoteOnEvent() && !ne.IsRestOnEvent();
    }),
    noteEvents.end());

  // 2. Go back until we are pointing to the first beat of a bar,
  //  and there is a note on or rest on event at that point.
  while (true)
  {
    const float timeAfter = iter->m_time;
    resumeTime = GoToFirstBeatOfBar(beats, iter);

std::cout << "**RESUME: Now at beat " 
  << iter->m_beat 
  << " of bar " 
  << iter->m_bar 
  << "\n";

    Assert(resumeTime <= timeAfter);

    if (FindNoteEventAtTime(resumeTime, noteEvents))
    {
      // If note found, that's good - we are finished. 
      break;
    }
    else if (iter == beats.begin())
    {
      // At start of song - we are done.
      return 0;
    }
    else
    {
      // No note found that the start of the bar.
      //  That means the bar starts with a note tied from the prev bar.
      //  OR there's a rest at the beginning of the bar.) 
      --iter; // go back to prev beat, i.e. the last beat of the prev bar.
    }
  }

  return resumeTime;
}

}

