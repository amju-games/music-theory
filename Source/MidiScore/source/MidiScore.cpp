// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <MidiFile.h>
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
  const Quantiser& quantiser)
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
      if (numBytes > 2)
      {
        e.m_dynamics.SetVelocity(static_cast<int>(mev[2]));
      }

      // TimeVal is NOT set yet! In this function we decide whether or
      //  not to split the note, and then assign TimeVals based on that.
      AppendNoteEventToEvents(tpq, e, events, ts);
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

std::string OutputTrack(
  int tpq, Events& events, TimeSig ts, KeySig ks, bool debug, int numBars, bool yesDynamics)
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

  if (yesDynamics)
    InsertDynamics(events);

  // Reset last dynamics string output (on prev track)
  Dynamics::SetLastDynamicsString();

  InsertChordMarkers(events);
//std::cout << "With chord markers: " << OutputEvents(events) << "\n";

  InsertBarLines(tpq, ts, events, numBars);
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

  bool preferFlatKey = true;
  ks = GuessKeySig(events, preferFlatKey); 
}

static Events GetEventsFromTrack(
  int tpq, const smf::MidiEventList& track, TimeSig ts, const Quantiser& quantiser)
{
  Events events;
  
  for (int event = 0; event < track.size(); event++) 
  {
    AddEventToVec(tpq, track[event], events, ts, quantiser);
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

static Events GetPitchEventsFromTrack(const smf::MidiEventList& track)
{
  // No quantising, note splitting etc - we just care about the pitches.
  // This is for clef and key sig guessing.
  const int TPQ = 256; // Arbitrary, probably should be high enough to avoid probs
  return GetEventsFromTrack(TPQ, track, TimeSig::TS_NONE, NullQuantiser());
}

// Helper type for looking up best fit string for a note duration
struct NoteMap {
    float target;
    std::string notation;
};

// Helper func for looking up best fit string for a note duration
// Generates the full lookup table including straight, dotted, and triplet notes
std::vector<NoteMap> generateNoteTable() 
{
    // Base straight notes ordered from smallest to largest
    std::vector<std::pair<float, std::string>> baseNotes = 
    {
        {1.0f / 16.0f, "qqqq"}, // 1/64 note
        {1.0f / 8.0f,  "qqq"},  // 1/32 note
        {1.0f / 4.0f,  "qq"},   // 1/16 note
        {1.0f / 2.0f,  "q"},    // 1/8 note
        {1.0f,         "c"},    // 1/4 note (Crotchet)
        {2.0f,         "m"},    // 1/2 note (Minim)
        {4.0f,         "sb"}    // Whole note (Semibreve)
    };

    std::vector<NoteMap> fullTable;

    for (const auto& base : baseNotes) {
        // 1. Add Straight Note
        fullTable.push_back({base.first, base.second});
        
        // 2. Add Dotted Note (Base * 1.5)
        fullTable.push_back({base.first * 1.5f, base.second + "."});
        
        // 3. Add Triplet Note (Base * 2/3)
        fullTable.push_back({base.first * (2.0f / 3.0f), base.second + " triplet"});
    }

    // Sort table by duration size to ensure predictable nearest-neighbour matching
    std::sort(fullTable.begin(), fullTable.end(), [](const NoteMap& a, const NoteMap& b) {
        return a.target < b.target;
    });

    return fullTable;
}

// Convert duration (in crotchet units) into a juliet-notation string. 
std::string quantiseFloatToNote(float duration) 
{
    static const std::vector<NoteMap> noteTable = generateNoteTable();

    // Catch extreme out-of-bounds values immediately
    if (duration < noteTable.front().target) return "qqqq-";
    if (duration > noteTable.back().target) return "sb+";

    float minDifference = std::numeric_limits<float>::max();
    std::string bestMatch = "";

    for (const auto& note : noteTable) {
        float difference = std::abs(duration - note.target);
        if (difference < minDifference) {
            minDifference = difference;
            bestMatch = note.notation;
        }
    }

    // Dynamic fallback check for values drifting past the table edges
    // dynamically handles values halfway between boundary thresholds
    if (duration < 0.04f) return "qqqq-"; 
    if (duration > 5.0f) return "sb+";

    return bestMatch;
}

std::string pitchStr(int pitch, bool preferFlats)
{
  static const std::string SHARPS[12] = 
    { "c", "c#", "d", "d#", "e", "f", "f#", "g", "g#", "a", "a#", "b" };
  static const std::string FLATS[12] = 
    { "c", "db", "d", "eb", "e", "f", "gb", "g", "ab", "a", "bb", "b" };

  std::string res;
  const int step = pitch % 12;
  res += (preferFlats ? FLATS[step] : SHARPS[step]);
  res += std::to_string(pitch / 12 - 1);
  res += " (" + std::to_string(pitch) + ")";
  return res;
}

std::string velStr(int vel)
{
  return std::to_string(vel);
}

// Get duration/pitch range for track as a string
std::string NoteRangeInTrack(int tpq, const smf::MidiEventList& track)
{
  if (track.size() == 0) return "-";

  std::string res;

  float minDuration = std::numeric_limits<float>::max();
  float maxDuration = 0;

  int minPitch = 129;
  int maxPitch = -1;

  int minVel = 129;
  int maxVel = -1;

  bool found = false;
  bool foundVel = false;

  for (int event = 0; event < track.size(); event++) 
  {
    const auto& mev = track[event];
    if (mev.isNoteOn()) 
    {
      auto numBytes = mev.size();
      if (numBytes > 1)
      { 
        found = true;  

        float duration = static_cast<float>(mev.getTickDuration());
        minDuration = std::min(minDuration, duration);
        maxDuration = std::max(maxDuration, duration); 

        int pitch = static_cast<int>(mev[1]);
        minPitch = std::min(minPitch, pitch);
        maxPitch = std::max(maxPitch, pitch);

        if (numBytes > 2)
        {
          foundVel = true;
          int vel = static_cast<int>(mev[2]);
          minVel = std::min(minVel, vel);
          maxVel = std::max(maxVel, vel);
        }
      }
    }   
  }

  if (!found) return "";

  minDuration /= static_cast<float>(tpq);
  maxDuration /= static_cast<float>(tpq);
  res = "  Duration range: " + 
    quantiseFloatToNote(minDuration) + " - "  + quantiseFloatToNote(maxDuration);

  const bool preferFlats = false; // TODO Get from key sig
  res += "\n  PitchRange: " + pitchStr(minPitch, preferFlats) + " - " + 
    pitchStr(maxPitch, preferFlats);

  if (foundVel)
  {
    res += "\n  Velocity range: " + velStr(minVel) + " - " + velStr(maxVel);
  }
  else
  {
    res += "\n  (No note velocities found.)";
  }

  return res;
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

static std::string InfoForMidiMsg(const smf::MidiEvent& msg)
{
  std::string res;
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

  return res;
}

std::string InfoString(smf::MidiFile& midifile)
{
  midifile.removeEmpties();
  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();
  int tracks = midifile.getTrackCount();
  std::string res = "TPQ: " + std::to_string(tpq) + 
    " number of tracks: " + std::to_string(tracks) + "\n";

  midifile.joinTracks();  // join all tracks just while we guess key sig
  if (midifile[0].getEventCount() == 0)
  {
    return "No events on any track!?\n";
  }

  bool preferFlatKey = true;
  auto ks = GuessKeySig(GetPitchEventsFromTrack(midifile[0]), preferFlatKey); 
  midifile.splitTracks();
  res += "Guessed key sig: " + KeySigString(ks) + "\n";

  for (int track = 0; track < tracks; track++) 
  {
    res += "Track " + std::to_string(track) + ":\n";

    int numEvents = CountNoteOnEventsInTrack(midifile[track]);
    res += "  Number of note on events: " + std::to_string(numEvents) + "\n";
    if (numEvents > 0)
    {
      Clef clef = GuessClef(GetPitchEventsFromTrack(midifile[track]));
      res += "  Guessed clef: " + ClefString(clef) + "\n";

      auto str = NoteRangeInTrack(tpq, midifile[track]);
      res += (str.empty() ? "" : str + "\n");
    }

    for (int i = 0; i < midifile[track].getEventCount(); i++) 
    {
      const auto& msg = midifile[track][i];
      res += InfoForMidiMsg(msg);
    }
  }

  return res;
}

std::string ToString(
  smf::MidiFile& midifile, 
  std::optional<int> track,
  std::string timeSig, 
  std::optional<int> keySig,
  std::optional<std::string> quant, 
  bool debug,
  float bpm) 
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

  // First pass: Join all tracks 
  midifile.joinTracks(); 
  Events allEvents = GetEventsFromTrack(tpq, midifile[0], ts, quantiser);
    // Timesig should be required, not an optional, so we know it here.
  if (allEvents.empty())
  {
    std::cout << "No events!\n";
    return "";
  }

  KeySig ks;
  TimeSig tsGuess;
  // Guess Key sig and Time sig from ALL events
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

  // If track is specified, just output that one track.
  if (track)
  {
    Events events = GetEventsFromTrack(tpq, midifile[*track], ts, quantiser);
    if (!events.empty()) 
    {
      res += "stave " + OutputTrack(tpq, events, ts, ks, debug, numBars, noDynamics) + "\n";
    }
  }
  else
  {
    // Output all (non-empty) tracks.
    int stave = 0;
    int numTracks = midifile.getNumTracks();
    for (int t = 0; t < numTracks; t++)
    {
      Events events = GetEventsFromTrack(tpq, midifile[t], ts, quantiser);
      if (events.empty()) continue;
      res += "// ** STAVE " + std::to_string(stave++) + " **\n";
      res += "stave " + OutputTrack(tpq, events, ts, ks, debug, numBars, noDynamics) + "\n";
    }
  }

  res += "\n";
  return res;
}
}

