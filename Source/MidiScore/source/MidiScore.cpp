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

  // If a rest, and the duration has not changed, just output
  //  immediate rest token.
  if (e.IsRest() && e.m_duration == prevDuration)
  {
    return "r"; // immediate rest token
  }

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
  midifile.splitTracks();
  int tracks = midifile.getTrackCount();
  
  for (int track = 0; track < tracks; track++) 
  {
    Events events = GetEventsFromTrack(tpq, midifile[track]);
    if (events.empty()) continue;
    res += "stave " + OutputTrack(tpq, events, ts, ks);
  }

  res += "\n";
  return res;
}
}

