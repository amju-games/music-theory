// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include <MidiFile.h>
#include "Anacrusis.h"
#include "Clef.h"
#include "Event.h"
#include "KeySig.h"
#include "MidiScore.h"
#include "Quantiser.h"
#include "TimeSig.h"

namespace MidiScore
{
static void AddEventToVec(
  int tpq, const smf::MidiEvent& mev, Events& events, TimeSig ts, 
  KeySig ks,
  const Quantiser& quantiser, int anacrusisTicks)
{
  if (mev.isNoteOn())
  {
    auto numBytes = mev.size();
    if (numBytes > 1)
    {   
      // Add note event to vec of events
      Event e;
      e.m_unquantisedStart = mev.tick;
      e.m_unquantisedDuration = mev.getTickDuration();
      e.m_end = e.m_start + e.m_duration;

      quantiser.QuantiseStartTime(tpq, e);

      // Set duration to the closest multiple of tpqs in the quant resolution.
      // But don't set the timeval to the closest timeval. That would
      //  obliterate crucial timing info -- we might need to split the
      //  note to capture its length.
      quantiser.QuantiseDuration(tpq, e);

      e.m_pitch = static_cast<int>(mev[1]);
      e.m_keySig = ks;

      if (numBytes > 2)
      {
        e.m_dynamics.SetVelocity(static_cast<int>(mev[2]));
      }

      // TimeVal is NOT set yet! In this function we decide whether or
      //  not to split the note, and then assign TimeVals based on that.
      const bool NO_SPLIT_ON_BEATS = false;
      AppendNoteEventToEvents(tpq, e, events, ts, NO_SPLIT_ON_BEATS, anacrusisTicks);
    }
  }
}

std::string OutputEvent(int& prevDuration, const Event& e)
{
  // If this event is a note, check the duration - only output if it
  //  has changed.
  if (e.IsNote() && e.m_duration == prevDuration)
  {
    // Duration the same -- just need to output pitch
    return e.NoteToStringNoDuration(); // also dynamics etc
  }

  // If a rest, and the duration has not changed, just output
  //  immediate rest token.
  if (e.IsRest() && e.m_duration == prevDuration)
  {
    return (e.m_isWholeBar ? "R" : "r"); // immediate rest token
  }

  if (e.m_duration > 0 || prevDuration < 0)
  {
    prevDuration = e.m_duration;
  }

  return e.ToString();
}

std::string OutputTrack(
  int tpq, Events& events, TimeSig ts, KeySig ks, bool debug, int numBars, 
  bool yesDynamics, bool yesTimeSetEvents, int anacrusisTicks, bool allClefs)
{
  if (events.empty())
  {
    return "";
  }

//std::cout << "Raw events: " << OutputEvents(events) << "\n";

  // Prepend time sig and key sig. TODO Later on consider looking for
  //  key sig changes, as we do for clefs.
  const int atStart = 0;
  events.insert(events.begin(), MakeKeySigEvent(ks, atStart)); 
  events.insert(events.begin(), MakeTimeSigEvent(ts, atStart)); 

  if (yesDynamics)
    InsertDynamics(events);

  // Reset last dynamics string output (on prev track)
  Dynamics::SetLastDynamicsString();

  InsertChordMarkers(events);
//std::cout << "With chord markers: " << OutputEvents(events) << "\n";

  InsertBarLines(tpq, ts, events, numBars, anacrusisTicks);
//std::cout << "With bar lines: " << OutputEvents(events) << "\n";

  // Get initial clef event and possibly changes; interleave these events.
  ClefChanges allClefChanges;
  GuessClef(events, tpq, anacrusisTicks, ts, allClefChanges, !allClefs);
  InsertClefs(events, allClefChanges);

  // Fill 'gaps' between note events with rests
  InsertRests(tpq, events, ts);
//std::cout << "With rests: " << OutputEvents(events) << "\n";

  if (yesTimeSetEvents)
    InsertTimeSetEvents(tpq, events);
//std::cout << "With time sets: " << OutputEvents(events) << "\n";

  if (debug)
  {
    return OutputEventsDebug(tpq, events);
  }

  return OutputEvents(events);
}

std::string OutputEventsDebug(int tpq, const Events& events)
{
  std::string res = "\n";

  // Traverse events. Output time val when it changes.
  int prevDuration = -1;

  // Zero-based bar numbers to match MakeScore
  res += "// BAR: 0\n";
  int barNum = 1; // next bar num 

  // So we can output times as crotchets from last bar line
  int lastBarStart = 0;

  for (int i = 0; i < events.size(); i++)
  {
    const Event& e = events[i];

    if (e.IsBarLine())
    {
      // Bar line events are the end of bars, but we output as if it's
      //  the start of the new bar - unless this is the final event.
      if (i == events.size() - 1)
      {
        res += "// Final bar line. ";
      }
      else 
      {
        res += "// BAR: " + std::to_string(barNum++);
      }
      res += " Start: " + std::to_string(e.m_start) + 
        " (" + std::to_string(e.m_start / tpq) + " crotchets):\n";

      lastBarStart = e.m_start;
    }
    else if (e.IsNote() || e.IsRest())
    {
      auto startFromBar = (e.m_start - lastBarStart) / tpq;
      std::stringstream ss;
      ss << startFromBar;
      res += "// (Start: " + ss.str() + " c in bar)";
      res += "  (Duration: " + e.DurationString() + "):\n";
    }
  
    // Output the event, even if just bar line... because
    //  MakeScore can read this debug format :)
    res += OutputEvent(prevDuration, e);
   
    res += "\n";
  }
  return res;
}

// For tests
std::string OutputNoteDurations(const Events& events)
{
  std::string res;

  for (int i = 0; i < events.size(); i++)
  {
    const auto& e = events[i];

    if (e.IsNote())
      res += e.DurationString();
    else
      res += e.ToString();

    if (i < (events.size() - 1))
      res += " ";
  }
  return res;
}

std::string OutputEvents(const Events& events)
{
  std::string res;

  // Traverse events. Output time val when it changes.
  int prevDuration = -1;
  for (int i = 0; i < events.size(); i++)
  {
    res += OutputEvent(prevDuration, events[i]);
    res += " ";
  }
  return res;
}

void GuessTimeSigAndKeySig(int tpq, const Events& events, TimeSig& ts, KeySig& ks)
{
  ts = GuessTimeSig(tpq, events); 

  const bool preferFlatKey = true; // TODO command line param?
  ks = GuessKeySig(events, preferFlatKey); 
}

Events GetEventsFromTrack(
  int tpq, const smf::MidiEventList& track, TimeSig ts, KeySig ks, 
  const Quantiser& quantiser,
  int anacrusisTicks)
{
  Events events;
  
  for (int event = 0; event < track.size(); event++) 
  {
    AddEventToVec(tpq, track[event], events, ts, ks, quantiser, anacrusisTicks);
  }   
  
  if (events.empty()) return events;

  // Sort by ascending start time, to minimise number of time jumps 
  std::sort(events.begin(), events.end(), 
    [](const Event& e1, const Event& e2) 
    {   
      return e1.m_start < e2.m_start; 
    }   
  ); 

  // Second quantisation pass over events, e.g. to chop durations so 
  //  there are no overlaps, if required
  quantiser.SecondPass(events);

  return events;
}

std::string ToString(
  smf::MidiFile& midifile, 
  std::optional<int> track,
  std::string timeSig, 
  std::optional<int> keySig,
  std::optional<std::string> quant, 
  bool debug,
  float bpm,
  std::optional<std::string> anacrusis,
  bool allClefs)
{
  std::string res;

  TimeVal quantRes = TimeVal::NONE;
  if (quant)
  {
    quantRes = GetTimeValFromString(*quant);
    if (quantRes == TimeVal::NONE)
    {
      std::cout << "Unrecognised quant resolution! Use one of sb/m/c/q/qq/qqq, or don't specify to not quantise.\n";
      return {};
    }

    std::cout << "// Quantising to: " << TimeValString(quantRes) << "\n";
  }

  // TODO Strategy pattern: create different quantiser type depending on 
  //  command line params, in MakeQuantiser. 
  // E.g. if no quant res set, create a null quantiser that does nothing.
  // We will start off with a quantiser impl that chops durations so there
  //  are no chords, then add a different impl later when required.
  const Quantiser& quantiser = MakeQuantiser(quantRes); 

  TimeSig ts;
  ts = GetTimeSigFromString(timeSig);

  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();
  std::cout << "// TPQ: " << tpq << "\n";

  int anacrusisTicks = GetAnacrusisTicks(anacrusis, tpq); 
  // number of tpq ticks in first, incomplete bar

  if (anacrusis)
  { 
     auto timeval = GetTimeValFromString(*anacrusis);
     // Assume anacrusis string is error checked in main() ?
     if (timeval == TimeVal::NONE)
     {
       std::cout << "Bad anacrusis duration.\n";
       return {};
     }
     // TODO This function doesn't handle dots, it's just a lookup
     anacrusisTicks = CalcTpqMultipleForTimeVal(tpq, timeval);
  }

  // First pass: Join all tracks 
  midifile.joinTracks(); 
  Events allEvents = GetEventsFromTrack(
    tpq, midifile[0], ts, KeySig::KS_SHARP_0, quantiser, anacrusisTicks);

  if (allEvents.empty())
  {
    std::cout << "No events!\n";
    return "";
  }

  KeySig ks;
  TimeSig tsGuess;
  // Guess Key sig (and possibly in future Time sig) from ALL events.
  // (We don't use the guessed time sig for now.)
  GuessTimeSigAndKeySig(tpq, allEvents, tsGuess, ks);

  // Override guess if value given
  if (keySig)
  {
    ks = IntToKeySig(*keySig);
  }

  int numBars = static_cast<int>(
    std::ceil(
    static_cast<float>(allEvents.back().m_end) / 
    static_cast<float>(tpq) / 
    static_cast<float>(BeatsInBar(ts))));

  if (anacrusis) 
  {
    std::cout << "// Anacrusis: " << *anacrusis << "\n";
  }

  std::cout << "// Num bars: " << numBars << "\n";

  // Give a rough page width using the number of bars
  res += "page-w " + std::to_string(6 * numBars) + "\n";

  res += "bpm " + (std::stringstream() << bpm).str() + "\n";

  // TODO do other first-pass things on all the events
  // E.g. create dynamics markers

  // 2nd pass: Un-join tracks and output each track 
  //  as a separate stave  
  midifile.splitTracks();

  const bool noDynamics = false;
  const bool noTimeSets = false;

  // If track is specified, just output that one track.
  if (track)
  {
    Events events = GetEventsFromTrack(tpq, midifile[*track], ts, ks, quantiser,
      anacrusisTicks);
    if (!events.empty()) 
    {
      res += "stave " + 
        OutputTrack(tpq, events, ts, ks, debug, numBars, noDynamics, noTimeSets,
          anacrusisTicks, allClefs) + "\n";
    }
  }
  else
  {
    // Output all non-empty tracks.
    int stave = 0;
    int numTracks = midifile.getNumTracks();
    for (int t = 0; t < numTracks; t++)
    {
      Events events = GetEventsFromTrack(tpq, midifile[t], ts, ks, quantiser,
        anacrusisTicks);
      if (events.empty()) continue;
      res += "// ** STAVE " + std::to_string(stave++) + " **\n";
      res += "stave " + 
        OutputTrack(tpq, events, ts, ks, debug, numBars, noDynamics, noTimeSets,
          anacrusisTicks, allClefs) + "\n";
    }
  }

  res += "\n";
  return res;
}
}

