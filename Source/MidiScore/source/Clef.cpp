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

namespace MidiScore
{
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

// --- 2. CONFIGURATION ---

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

// --- 3. PIPELINE HELPERS ---

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
  int measures_per_window
)
{
  std::map<int, std::vector<int>> chunks;
  
  // Calculate exact footprint of a single measure in MIDI ticks
  int ticks_per_measure = (ticks_per_quarter * 4 * numerator) / denominator;
  int window_size_ticks = ticks_per_measure * measures_per_window;

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
    int chunk_index = event.m_unquantisedStart / window_size_ticks;
    chunks[chunk_index].push_back(event.m_pitch);
  }
  return chunks;
}

// --- 4. CORE TIMELINE ORCHESTRATOR ---

std::vector<ClefChange> GenerateClefChanges(
  const Events& constevents, 
  int ticks_per_quarter, 
  int numerator, 
  int denominator,
  bool justTrebleAndBass
)
{
  std::vector<ClefChange> timeline;
  if (constevents.empty())
  {
std::cout << "Clef guessing... no events\n";
    return timeline;
  }

  // Evaluate the score in chunks of 4 full measures
  const int measures_per_window = 4;
  int ticks_per_measure = (ticks_per_quarter * 4 * numerator) / denominator;
  int window_size_ticks = ticks_per_measure * measures_per_window;

  auto time_chunks = SegmentPitchesByMeasures(
    constevents, ticks_per_quarter, numerator, denominator, measures_per_window
  );
  if (time_chunks.empty())
  {
    return timeline;
  }
  std::vector<Clef> chunk_winners;

  // Establish the starting clef from the initial measure block
  Clef current_clef = EvaluateSingleWindow(time_chunks[0], Clef::TREBLE, justTrebleAndBass);
  timeline.push_back({0, current_clef});
  chunk_winners.push_back(current_clef);

  Clef pending_clef = current_clef;
  int consecutive_wins = 0;
  const int persistence_threshold = 2; // Must hold out for 2 windows (8 measures)

  for (const auto& [chunk_index, pitches] : time_chunks)
  {
    if (chunk_index == 0)
    {
      continue; 
    }

    Clef chunk_winner = EvaluateSingleWindow(pitches, current_clef, justTrebleAndBass);
    chunk_winners.push_back(chunk_winner);

    if (chunk_winner != current_clef)
    {
      if (chunk_winner == pending_clef)
      {
        consecutive_wins++;
      }
      else
      {
        pending_clef = chunk_winner;
        consecutive_wins = 1;
      }

      // Threshold crossed! Safely push a clean downbeat-aligned clef change
      if (consecutive_wins >= persistence_threshold)
      {
        current_clef = chunk_winner;
        int downbeat_tick = chunk_index * window_size_ticks;
        timeline.push_back({downbeat_tick, current_clef});
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

Clef GuessClef(const Events& e, int tpq, TimeSig ts, ClefChanges& allClefChanges,
  bool justTrebleAndBass)
{
  int num = Numerator(ts);
  int denom = Denominator(ts);
  if (ts == TimeSig::TS_NONE)
  {
    num = 4; denom = 4; // default to 4/4, not 1/1
  }

  allClefChanges = GenerateClefChanges(e, tpq, num, denom, justTrebleAndBass);
  if (allClefChanges.empty())
  {
std::cout << "// Defaulting to clef-t!\n";
    // Just in case we don't have any clefs:
    allClefChanges.push_back({0, Clef::TREBLE});
  }
  return allClefChanges.front().clef;
}
}

