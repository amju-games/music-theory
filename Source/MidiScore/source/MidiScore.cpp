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
    // Add note event to vec
    Event e;
    e.m_start = mev.tick;
    e.m_duration = mev.getTickDuration();
    e.m_end = e.m_start + e.m_duration;

    e.SetTimeVal(tpq);

    int numBytes = mev.size();
    if (numBytes > 2)
    {   
      e.m_pitch = static_cast<int>(mev[1]);
      e.m_velocity = static_cast<int>(mev[2]);
    }

    events.push_back(e);
  }
}

std::string TimeValString(TimeVal t)
{
  if (t == TimeVal::SEMIQUAVER) return "qq";
  if (t == TimeVal::QUAVER) return "q";
  if (t == TimeVal::CROTCHET) return "c";
  if (t == TimeVal::MINIM) return "m";
  return "sb";
}

std::string OutputEvent(const Event& e)
{
  std::string res;
  if (e.m_isRest)
  {
    res = TimeValString(e.m_timeVal) + "r" + (e.m_dots > 0 ? std::string(e.m_dots, '.') : "");
  }
  {
    res = "<" + TimeValString(e.m_timeVal) + (e.m_dots > 0 ? std::string(e.m_dots, '.') : "");
    res += "> " + std::to_string(e.m_pitch);
  }
  return res;
}

std::string OutputEvent(const Event& prev, const Event& e)
{
  if (e.m_isRest || prev.m_isRest) // either is rest - output this event in full
  {
    return OutputEvent(e);
  }
  if (e.m_duration == prev.m_duration)
  {
    // Duration the same -- just need to output pitch
    return std::to_string(e.m_pitch);
  }
  return OutputEvent(e);
}

std::string OutputTrack(int tpq, Events& events)
{
  if (events.empty())
  {
    return "";
  }

  TimeSig ts = GuessTimeSig(tpq, events);

  Clef clef = GuessClef(events); // from pitch range

  bool preferFlatKey = true;
  KeySig ks = GuessKeySig(events, preferFlatKey); // from all pitches

  std::string res;
  res += ClefString(clef) + " ";
  res += TimeSigString(ts) + " ";
  res += KeySigString(ks) + " ";

  // Fill 'gaps' between note events with rests
  FillGapsWithRests(tpq, events);

  // Traverse events. Output time val and pitch when either changes.
  res += OutputEvent(events.front()) + " ";
  for (int i = 1; i < events.size(); i++)
  {
    res += OutputEvent(events[i - 1], events[i]);
    res += " ";
  }
  res += "\n";
  return res;
}


std::string ToString(smf::MidiFile& midifile)
{
  std::string res;

  // should we do this? Perhaps we need to do one pass with, 
  //  to get all "verticals", and then one without, to get the different voices.
  // TODO Decide whether or not to join the tracks. If we do, (and if we don't)
  //  -- we need to detect chords.
//  midifile.joinTracks(); 

  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();

  int tracks = midifile.getTrackCount();
  
  for (int track = 0; track < tracks; track++) 
  {
std::cout << "Track: " << track << "\n";

    Events events;
    for (int event = 0; event < midifile[track].size(); event++) 
    {
      AddEventToVec(tpq, midifile[track][event], events);
    }   
    res += OutputTrack(tpq, events);
  }

  return res;
}
}

