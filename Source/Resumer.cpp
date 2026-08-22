// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <iostream>
#include <AmjuAssert.h>
#include "Resumer.h"

//#define RESUMER_DEBUG

namespace Amju
{
// iter points to a beat in the BeatVec for the piece.
// Decrement iter until we point to the first beat in a bar.
// Set time to the time of the beat iter points to.
// NB we decrement the iterator in the calling code - note pass by ref.
float Resumer::GoToFirstBeatOfBar(
  const BeatVec& beats, 
  BeatVec::const_iterator& iter) // NON CONST REF
{ 
  if (beats.empty() || iter == beats.end()) 
  {
    return 0;
  }

  // Go back to beat 1 in the current bar
  while (iter != beats.begin() && iter->m_beat > 1) 
  { 
    --iter; 
  } 
 
  return iter->m_time;
}   

// Look for a note event at the given time; return true if found.
// This is used to check if there is a note event at the time
//  of the first beat of the bar. If not, we can go back to the prev bar.
bool Resumer::FindNoteEventAtTime(float time, const NoteEvents& noteEvents,
  float epsilon)
{
  if (noteEvents.empty() || epsilon < 0.0f) {
    return false;
  }

  // Binary search to find the first event with m_time >= (time - epsilon)
  auto it = std::lower_bound(
      noteEvents.begin(), 
      noteEvents.end(), 
      time - epsilon,
      [](const NoteEvent& event, float value) {
          return event.m_time < value;
      }
  );

  // Check if the found element is within [time - epsilon, time + epsilon]
  if (it != noteEvents.end() && std::abs(it->m_time - time) <= epsilon) 
  {
    return true;
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
#ifdef RESUMER_DEBUG
std::cout << "*** FIND RESUME POINT. Pause time: " << pauseTime << "\n";
#endif

  float resumeTime = pauseTime;

  // Work out how far back we should go from the resume time.
  // We want to find the start of the current bar.
  // If we're in the first bar, we should just restart the game round. 

  // Find the beat closest to resumeTime:
  // 1. Find the next beat _after_ resumeTime
  auto iter = NextBeatAfterTime(beats, resumeTime);

  if (iter == beats.begin())
  {
    return 0;
  }
  else if (iter == beats.end())
  {
    // We must have reached the end of the song. 
#ifdef RESUMER_DEBUG
std::cout << "VERY STRANGE, on resuming, we seem to be at the end of the song?\n";
#endif

    // Return a super high value for resume time, so GSHero will detect the
    //  end of the round.
    return HUGELY_LONG_TIME;
  }

  // We have got the next beat after resumeTime.
  Assert(iter->m_time > resumeTime);

  // Go to the prev beat: this fixes the bug where the beat iter points
  //  to is the first beat of the next bar.
  --iter;

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

    Assert(resumeTime <= timeAfter);
    Assert(iter->m_beat == 1);

#ifdef RESUMER_DEBUG
std::cout << "  ** Resume point: found 1st beat of bar " << iter->m_bar << "\n";
#endif

    const float epsilon = 0.000001f;
    if (FindNoteEventAtTime(resumeTime, noteEvents, epsilon))
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

#ifdef RESUMER_DEBUG
std::cout << "*** FIND RESUME POINT. Resume time: " << resumeTime << "\n";
#endif

  return resumeTime;
}
}

