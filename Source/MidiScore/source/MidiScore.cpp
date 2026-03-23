// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <MidiFile.h>
#include "Clef.h"
#include "Event.h"
#include "KeySig.h"
#include "MidiScore.h"
#include "TimeSig.h"

namespace MidiScore
{
static TimeVal s_quantRes = TimeVal::QQQ;

void AddEventToVec(int tpq, const smf::MidiEvent& mev, Events& events)
{
  if (mev.isNoteOn())
  {
    int numBytes = mev.size();
    if (numBytes > 1)
    {   
      // Add note event to vec of events
      Event e;
      e.m_unquantisedStart = mev.tick;
      e.m_unquantisedDuration = mev.getTickDuration();
      e.m_end = e.m_start + e.m_duration;

      e.QuantiseStartTime(tpq, s_quantRes);

      // Set duration to the closest multiple of tpqs in the quant resolution.
      // But don't set the timeval to the closest timeval. That would
      //  obliterate crucial timing info -- we might need to split the
      //  note to capture its length.
      e.QuantiseDuration(tpq, s_quantRes);

      e.m_pitch = static_cast<int>(mev[1]);
      if (numBytes > 2)
      {
        e.m_dynamics.SetVelocity(static_cast<int>(mev[2]));
      }

      // TimeVal is NOT set yet! In this function we decide whether or
      //  not to split the note, and then assign TimeVals based on that.
      AppendNoteEventToEvents(tpq, e, events);
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
    return "r"; // immediate rest token
  }

  prevDuration = e.m_duration;

  return e.ToString();
}

std::string OutputTrack(int tpq, Events& events, TimeSig ts, KeySig ks, bool debug)
{
  if (events.empty())
  {
    return "";
  }

//std::cout << "Raw events: " << OutputEvents(events) << "\n";

  Clef clef = GuessClef(events); 

  std::string res;
  res += ClefString(clef) + " ";
  res += TimeSigString(ts) + " ";
  res += KeySigString(ks) + " ";

  InsertDynamics(events);

  // Reset last dynamics string output (on prev track)
  Dynamics::SetLastDynamicsString();

  InsertChordMarkers(events);
//std::cout << "With chord markers: " << OutputEvents(events) << "\n";

  InsertBarLines(tpq, ts, events);
//std::cout << "With bar lines: " << OutputEvents(events) << "\n";

  // Fill 'gaps' between note events with rests
  InsertRests(tpq, events, ts);
//std::cout << "With rests: " << OutputEvents(events) << "\n";

  InsertTimeSetEvents(tpq, events);
//std::cout << "With time sets: " << OutputEvents(events) << "\n";

  if (debug)
  {
    return OutputEventsDebug(tpq, res, events);
  }

  return res + OutputEvents(events);
}

std::string OutputEventsDebug(int tpq, const std::string& bar1Preamble, const Events& events)
{
  std::string res = "\n";

  // Traverse events. Output time val when it changes.
  int prevDuration = -1;

  // Zero-based bar numbers to match MakeScore
  res += "// BAR: 0\n" + bar1Preamble + "\n";
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
      float startFromBar = (e.m_start - lastBarStart) / tpq;
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
  // This has to be passed in because it MUST NOT be different across tracks
  ts = GuessTimeSig(tpq, events); // or user can specify - TODO

  // This has to be passed in because it shouldn't be different across tracks
  bool preferFlatKey = true;
  ks = GuessKeySig(events, preferFlatKey); // or user can specify - TODO
}

Events GetEventsFromTrack(int tpq, const smf::MidiEventList& track)
{
  Events events;
  
  for (int event = 0; event < track.size(); event++) 
  {
    AddEventToVec(tpq, track[event], events);
  }   
  
  if (events.empty()) return events;

  // Sort by ascending start time, to minimise number of time jumps 
  std::sort(events.begin(), events.end(), 
    [](const Event& e1, const Event& e2) 
    {   
      return e1.m_start < e2.m_start; 
    }   
  ); 

  return events;
}

int CountNoteOnEventsInTrack(const smf::MidiEventList& track)
{
  int count = 0;
  for (int event = 0; event < track.size(); event++) 
  {
    if (track[event].isNoteOn()) count++;
  }
  return count;
}

std::string InfoString(smf::MidiFile& midifile)
{
  midifile.removeEmpties();
  midifile.doTimeAnalysis();
  const int tpq = midifile.getTicksPerQuarterNote();
  int tracks = midifile.getTrackCount();
  std::string res = "TPQ: " + std::to_string(tpq) + 
    " number of tracks: " + std::to_string(tracks) + "\n";

  for (int track = 0; track < tracks; track++) 
  {
    res += "Track " + std::to_string(track) + ":\n";
    
    res += "  Number of note on events: " + 
      std::to_string(CountNoteOnEventsInTrack(midifile[track])) + "\n";

    for (int i = 0; i < midifile[track].getEventCount(); i++) 
    {
        const auto& msg = midifile[track][i];
        if (msg.isTrackName())
        {
           std::string content = msg.getMetaContent();
           res += "  Track name: " + content + "\n";
        }
 
        if (msg.isKeySignature()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Key Signature: " + content + "\n";
        }
 
        if (msg.isTimeSignature()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Time Signature: " + content + "\n";
        }
 
        if (msg.isTempo()) 
        {
           std::string content = std::to_string(msg.getTempoBPM());
           res += "  Tempo: " + content + " BPM\n";
        }
 
        if (msg.isMarkerText()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Marker text: " + content + "\n";
        }
 
        if (msg.isLyricText()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Lyric text: " + content + "\n";
        }
 
        if (msg.isInstrumentName()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Instrument name: " + content + "\n";
        }
 
        if (msg.isCopyright()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Copyright: " + content + "\n";
        }
 
        if (msg.isText()) 
        {
           std::string content = msg.getMetaContent();
           res += "  Text: " + content + "\n";
        }
    }
  }

  return res;
}

std::string ToString(
  smf::MidiFile& midifile, 
  std::optional<int> track,
  std::optional<std::string> timeSig, 
  std::optional<int> keySig,
  std::optional<std::string> quant, 
  bool debug,
  std::optional<float> bpm) 
{
  std::string res;

  if (quant)
  {
    s_quantRes = GetTimeValFromString(*quant);
    std::cout << "// Quantising to: " << TimeValString(s_quantRes) << "\n";
  }

  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();
  std::cout << "// TPQ: " << tpq << "\n";

  // First pass: Join all tracks 
  midifile.joinTracks(); 
  Events allEvents = GetEventsFromTrack(tpq, midifile[0]);
  if (allEvents.empty())
  {
    std::cout << "No events!\n";
    return "";
  }

  TimeSig ts;
  KeySig ks;
  // Guess Key sig and Time sig from ALL events
  GuessTimeSigAndKeySig(tpq, allEvents, ts, ks);

  // Override guesses if values have been given
  if (timeSig)
  {
    ts = GetTimeSigFromString(*timeSig);
  }

  if (keySig)
  {
    ks = IntToKeySig(*keySig);
  }

  int numBars = 
    std::ceil(
    static_cast<float>(allEvents.back().m_end) / 
    static_cast<float>(tpq) / 
    static_cast<float>(BeatsInBar(ts)));

  std::cout << "// Num bars: " << numBars << "\n";

  // Give a rough page width using the number of bars
  res += "page-w " + std::to_string(4 * numBars) + "\n";

  if (bpm)
  {
    res += "bpm " + (std::stringstream() << *bpm).str() + "\n";
  }

  // TODO do other first-pass things on all the events
  // E.g. create dynamics markers

  // 2nd pass: Un-join tracks and output each track 
  //  as a separate stave  
  midifile.splitTracks();

  // If track is specified, just output that one track.
  if (track)
  {
    Events events = GetEventsFromTrack(tpq, midifile[*track]);
    if (!events.empty()) 
    {
      res += "stave " + OutputTrack(tpq, events, ts, ks, debug) + "\n";
    }
  }
  else
  {
    // Output all (non-empty) tracks.
    int stave = 0;
    int numTracks = midifile.getNumTracks();
    for (int t = 0; t < numTracks; t++)
    {
      Events events = GetEventsFromTrack(tpq, midifile[t]);
      if (events.empty()) continue;
      res += "// ** STAVE " + std::to_string(stave++) + " **\n";
      res += "stave " + OutputTrack(tpq, events, ts, ks, debug) + "\n";
    }
  }

  res += "\n";
  return res;
}
}

