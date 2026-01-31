// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <map>
#include <MidiFile.h>
#include "Clef.h"
#include "Event.h"
#include "KeySig.h"
#include "MidiScore.h"
#include "TimeSig.h"

namespace MidiScore
{
void AddEventToVec(int tpq, const smf::MidiEvent& mev, Events& events)
{
  if (mev.isNoteOn())
  {
    int numBytes = mev.size();
    if (numBytes > 1)
    {   
      // Add note event to vec of events
      Event e;
      e.m_start = mev.tick;
      e.m_duration = mev.getTickDuration();
      e.m_end = e.m_start + e.m_duration;

      e.SetTimeVal(tpq);

      e.m_pitch = static_cast<int>(mev[1]);
      if (numBytes > 2)
      {
        e.m_dynamics.SetVelocity(static_cast<int>(mev[2]));
      }
      AppendNoteEventToEvents(e, events);
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

  if (e.IsNote())  
    prevDuration = e.m_duration;

  return e.ToString();
}

std::string OutputTrack(int tpq, Events& events, TimeSig ts, KeySig ks)
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

  InsertChordMarkers(events);
//std::cout << "With chord markers: " << OutputEvents(events) << "\n";

  InsertBarLines(tpq, ts, events);
//std::cout << "With bar lines: " << OutputEvents(events) << "\n";

  // Fill 'gaps' between note events with rests
  InsertRests(tpq, events);
//std::cout << "With rests: " << OutputEvents(events) << "\n";

  InsertTimeSetEvents(tpq, events);
//std::cout << "With time sets: " << OutputEvents(events) << "\n";

  return res + OutputEvents(events);
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
  res += "\n";
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

  return events;
}

std::string ToString(smf::MidiFile& midifile)
{
  std::string res;

  // should we do this? Perhaps we need to do one pass with, 
  //  to get all "verticals", and then one without, to get the different voices.
  // Also, if we are guessing the key sig and time sigs, we should do it on
  //  all the notes across all the tracks, then use those same guesses
  //  for every stave. 
  // 
  // TODO Decide whether or not to join the tracks. If we do, (and if we don't)
  //  -- we need to detect chords.
//  midifile.joinTracks(); 

  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();

  midifile.joinTracks(); // Join all tracks and do a first pass on all the events
  Events allEvents = GetEventsFromTrack(tpq, midifile[0]);
  TimeSig ts;
  KeySig ks;
  GuessTimeSigAndKeySig(tpq, allEvents, ts, ks);
  // TODO do other first-pass things on all the events
  // E.g. create dynamics markers

  // 2nd pass: Un-join tracks and output each track -- TODO as a separate stave  
//  midifile.splitTracks();
  int tracks = midifile.getTrackCount();
  
  for (int track = 0; track < tracks; track++) 
  {
    Events events = GetEventsFromTrack(tpq, midifile[track]);
    res += OutputTrack(tpq, events, ts, ks);
  }

  return res;
}
}

