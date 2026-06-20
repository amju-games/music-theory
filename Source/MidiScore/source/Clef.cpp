// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include "Clef.h"
#include "TimeSig.h"

//#define DEBUG_SHOW_CHUNK_WINNERS

namespace MidiScore
{
Event MakeClefChange(Clef clef, int time)
{
  Event e;
  e.m_type = EventType::CLEF;
  e.m_clef = clef;
  e.m_start = time;
  e.m_end = time;
  return e;
}

std::string ClefString(Clef clef)
{
  std::array<std::string, 4> CLEFS = 
  {
    "clef-t", "clef-b", "clef-alto", "clef-tenor"
  };
  return CLEFS[static_cast<int>(clef)];
}

struct ClefProfile
{
  Clef clef_type;
  int min_comfortable;  // Bottom of the standard staff
  int max_comfortable;  // Top of the standard staff
  int ideal_center;     // The middle line of the staff
};

const std::vector<ClefProfile>& GetClefProfiles(bool justTrebleAndBass)
{
  static const std::vector<ClefProfile> tbProfiles = {
    { Clef::TREBLE, 57, 84, 71 }, // Staff: E4 (64) to F5 (77). Middle: B4 (71)
    { Clef::BASS,   36, 64, 50 }, // Staff: G2 (43) to A3 (57). Middle: D3 (50)
  };

  static const std::vector<ClefProfile> allProfiles = {
    { Clef::TREBLE, 57, 84, 71 }, // Staff: E4 (64) to F5 (77). Middle: B4 (71)
    { Clef::BASS,   36, 64, 50 }, // Staff: G2 (43) to A3 (57). Middle: D3 (50)
    { Clef::ALTO,   50, 72, 60 }, // Staff: F3 (53) to G4 (67). Middle: C4 (60)
    { Clef::TENOR,  47, 69, 57 }  // Staff: D3 (50) to E4 (64). Middle: A3 (57)
  };

  return (justTrebleAndBass ? tbProfiles : allProfiles);
}

float CalculateProfileScore(const std::vector<int>& pitches, const ClefProfile& profile)
{
  if (pitches.empty())
  {
    return 0.0f;
  }

  float score = 0.0f;
  for (int pitch : pitches)
  {
    // Give points if the note fits comfortably on this staff
    if (pitch >= profile.min_comfortable && pitch <= profile.max_comfortable)
    {
      score += 1.0f;
    }
    
    // Add proximity bonus the closer the note is to the staff's center line
    int distance = std::abs(pitch - profile.ideal_center);
    score += 1.0f / (1.0f + static_cast<float>(distance)); 
  }
  return score;
}

Clef EvaluateSingleWindow(const std::vector<int>& pitches, Clef default_fallback,
  bool justTrebleAndBass)
{
  if (pitches.empty())
  {
    return default_fallback;
  }

  const auto& profiles = GetClefProfiles(justTrebleAndBass);
  Clef winning_clef = default_fallback;
  float highest_score = -1.0f;

  for (const auto& profile : profiles)
  {
    float score = CalculateProfileScore(pitches, profile);
    if (score > highest_score)
    {
      highest_score = score;
      winning_clef = profile.clef_type;
    }
  }
  return winning_clef;
}

std::map<int, std::vector<int>> SegmentPitchesByMeasures(
  const Events& events, 
  int ticks_per_quarter, 
  int numerator, 
  int denominator, 
  int measures_per_window,
  int anacrusisTicks
)
{
  std::map<int, std::vector<int>> chunks;
  
  // Calculate exact footprint of a single measure in MIDI ticks
  int ticks_per_bar = (ticks_per_quarter * 4 * numerator) / denominator;
  int window_size_ticks = ticks_per_bar * measures_per_window;

  if (window_size_ticks <= 0)
  {
    return chunks;
  }

  for (const auto& event : events)
  {
    if (event.m_pitch == 0)
    {
      continue;
    }
    
    // Group into time chunks
    // Chop off any anacrusis:
    //      . . | . . . . | . . . . |  =>  . . | . . . . | . . . . |
    // ch. -1--> ch. 0---> ch. 1--->       ch. 0 -------> ch. 1--->
    //
    // Subtract anacrusis time from event start; anything that is then
    //  negative just goes into chunk 0.
    int chunk_index = std::max(0,
      (event.m_unquantisedStart - anacrusisTicks) / window_size_ticks);
    assert(chunk_index >= 0);

    chunks[chunk_index].push_back(event.m_pitch);
  }
  return chunks;
}

std::vector<ClefChange> GenerateClefChanges(
  const Events& events, 
  int ticks_per_quarter, 
  int anacrusisTicks,
  int numerator, 
  int denominator,
  bool justTrebleAndBass,
  int bars_per_chunk,
  int threshold
)
{
  std::vector<ClefChange> timeline;
  if (events.empty())
  {
    return timeline;
  }

  // Evaluate the score in chunks -- currently a chunk is 4 bars. 
  int ticks_per_bar = (ticks_per_quarter * 4 * numerator) / denominator;
  int window_size_ticks = ticks_per_bar * bars_per_chunk;

  auto time_chunks = SegmentPitchesByMeasures(
    events, ticks_per_quarter, numerator, denominator, bars_per_chunk,
    anacrusisTicks);
  if (time_chunks.empty())
  {
    return timeline;
  }
  std::vector<Clef> chunk_winners; // for reporting only

  // Establish the starting clef from the initial measure block
  Clef current_clef = EvaluateSingleWindow(
    time_chunks[0], Clef::TREBLE, justTrebleAndBass);
  timeline.push_back(MakeClefChange(current_clef, 0));
  chunk_winners.push_back(current_clef);

  Clef pending_clef = current_clef;
  int consecutive_wins = 0;

  for (const auto& [chunk_index, pitches] : time_chunks)
  {
    if (chunk_index == 0)
    {
      continue; 
    }

    Clef chunk_winner = EvaluateSingleWindow(
      pitches, current_clef, justTrebleAndBass);
    chunk_winners.push_back(chunk_winner); // for reporting only

    if (chunk_winner != current_clef)
    {
      if (chunk_winner == pending_clef) 
      {
        // same as previous chunk, and not the currently prevailing clef
        consecutive_wins++;
      }
      else
      {
        // Not the currently prevailing clef, but different again to previous
        pending_clef = chunk_winner;
        consecutive_wins = 1;
      }

      // Threshold crossed! Safely push a clean downbeat-aligned clef change
      if (consecutive_wins >= threshold)
      {
        current_clef = chunk_winner;
        // Calc time to add clef: get the first chunk where we change; 
        //  re-add anacrusis, which we subtracted in SegmentPitchesByMeasures.
        int downbeat_tick = (chunk_index - threshold + 1) * window_size_ticks +
          anacrusisTicks;
        timeline.push_back(MakeClefChange(current_clef, downbeat_tick));
        consecutive_wins = 0;
      }
    }
    else
    {
      consecutive_wins = 0;
      pending_clef = current_clef;
    }
  }

#ifdef DEBUG_SHOW_CHUNK_WINNERS
  std::cout << "// Chunk winners: ";
  for (Clef c : chunk_winners) std::cout << ClefString(c) << " ";
  std::cout << "\n";
#endif

  return timeline;
}

Clef GuessClef(const Events& e, int tpq, 
  int anacrusisTicks,
  TimeSig ts, ClefChanges& allClefChanges,
  bool justTrebleAndBass,
  int barsPerChunk, 
  int threshold)
{
  int num = Numerator(ts);
  int denom = Denominator(ts);
  if (ts == TimeSig::TS_NONE)
  {
    num = 4; denom = 4; // default to 4/4, not 1/1
  }

  allClefChanges = GenerateClefChanges(e, tpq, anacrusisTicks, num, denom, justTrebleAndBass, barsPerChunk, threshold);
  if (allClefChanges.empty())
  {
    // Just in case we don't have any clefs:
    allClefChanges.push_back(MakeClefChange(Clef::TREBLE, 0));
  }
  return allClefChanges.front().m_clef;
}

void InsertClefs(Events& events, const ClefChanges& cc)
{
  // Interleave events and clef changes:
  // Add clef changes immediately after a bar line, except for the
  //  0th clef, which gets added before everything.
  // TODO remove or defer clef changes that split tied notes.

  if (cc.empty()) return;

  Events newEvents;
  int clefIndex = 0;
  for (const Event& e : events)
  {
    // Put the 0th clef right at the start. For subsequent clef changes, 
    //  we expect bar lines and clef changes to have the same start time.
    if (clefIndex == 0)
    {
      // Insert 0th clef before any other event
      const auto& clefChange = cc[clefIndex];
      assert(clefChange.m_start == 0);
      newEvents.push_back(clefChange);
      clefIndex = 1;
      newEvents.push_back(e);
    } 
    else if (clefIndex < cc.size() && // any clefs remaining
             // Clef change on bar line, that's good
             e.m_start == cc[clefIndex].m_start &&   
             e.IsBarLine()) 
    {
      // Insert clef after bar line
      newEvents.push_back(e);
      // TODO Check next event, in case it's a tie, (and maybe there are other types
      //  we can't split with a clef change?!)
      newEvents.push_back(cc[clefIndex]);
      ++clefIndex;
    }
    else if (clefIndex < cc.size() && // any clefs remaining
             // There was no bar line where the clef change fell. Move on to the next.
             e.m_start > cc[clefIndex].m_start)
    {
      // TODO We could push the clef change to the next bar, rather than just skipping it?
      newEvents.push_back(e);
      ++clefIndex;
    }
    else
    {
      newEvents.push_back(e);
    }
  }
  // We might have skipped clef changes, if they couldn't be added.
  assert(newEvents.size() >= events.size());
  assert(newEvents.size() <= events.size() + cc.size());
  events = newEvents;
}
}

