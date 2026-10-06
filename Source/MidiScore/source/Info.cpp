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
#include "Info.h"
#include "KeySig.h"
#include "MidiScore.h"
#include "Quantiser.h"
#include "TimeSig.h"

namespace MidiScore
{
// Helper type for looking up best fit string for a note duration
struct NoteMap 
{
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
static std::string quantiseFloatToNote(float duration)
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

static std::string pitchStr(int pitch, bool preferFlats)
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

static std::string velStr(int vel)
{
  return std::to_string(vel);
}

// Get duration/pitch range for track as a string
static std::string NoteRangeInTrack(int tpq, const smf::MidiEventList& track)
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

  std::set<int> channels;

  for (int event = 0; event < track.size(); event++)
  {
    const auto& mev = track[event];
    if (mev.isNoteOn())
    {
      auto numBytes = mev.size();

      // Get the channel (0-15)
      const int channel = mev.getChannel();
      channels.insert(channel); // add to set of channels

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
  res += "\n  Pitch range: " + pitchStr(minPitch, preferFlats) + " - " +
    pitchStr(maxPitch, preferFlats);

  if (foundVel)
  {
    res += "\n  Velocity range: " + velStr(minVel) + " - " + velStr(maxVel);
  }
  else
  {
    res += "\n  (No note velocities found.)";
  }

  if (channels.empty())
  {
    res += "\n  Couldn't find any channel info.";
  }
  else
  {
    res += std::string("\n  Channel") + ((channels.size() == 1) ? "" : "s") +
      " (0-based) used in this track: ";
    for (int ch : channels)
    {
      res += std::to_string(ch) + " ";
    }
  }

  return res;
}

static Events GetPitchEventsFromTrack(const smf::MidiEventList& track)
{
  // No quantising, note splitting etc - we just care about the pitches.
  // This is for clef and key sig guessing.
  const int TPQ = 256; // Arbitrary, probably should be high enough to avoid probs
  const int ANACRUSIS_TICKS = 0; // we don't care about timing
  return GetEventsFromTrack(
    TPQ, track, TimeSig::TS_NONE, KeySig::KS_SHARP_0, NullQuantiser(),
    ANACRUSIS_TICKS);
}

static int CountNoteOnEventsInTrack(const smf::MidiEventList& track)
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

static std::string InfoStringForOneTrack(
  smf::MidiFile& midifile,
  int track,
  int anacrusisTicks,
  std::optional<std::string> optionalTimeSig,
  bool allClefs)
{
  std::string res;
  res += "Track " + std::to_string(track) + ":\n";

  const int tpq = midifile.getTicksPerQuarterNote();

  for (int i = 0; i < midifile[track].getEventCount(); i++)
  {
    const auto& msg = midifile[track][i];
    res += InfoForMidiMsg(msg);
  }

  int numEvents = CountNoteOnEventsInTrack(midifile[track]);
  res += "  Number of note on events: " + std::to_string(numEvents) + "\n";
  if (numEvents > 0)
  {
    const auto pitches = GetPitchEventsFromTrack(midifile[track]);

    const bool preferFlatKey = true; // TODO should be command line param
    auto ks = GuessKeySig(pitches, preferFlatKey);
    res += "  Guessed key sig: " + KeySigString(ks) + "\n";

    ClefChanges allClefChanges;
    // Helpful for clef changes to know the time sig and anacrusis - TODO
    TimeSig ts = TimeSig::TS_4_4; // default
    if (optionalTimeSig) ts = GetTimeSigFromString(*optionalTimeSig);
    GuessClef(pitches, tpq, anacrusisTicks, ts, allClefChanges, !allClefs);
    res += "  Guessed clefs: ";
    for (const auto& cc : allClefChanges)
    {
       res += ClefString(cc.m_clef) + " ";
    }
    res += "\n";

    auto str = NoteRangeInTrack(tpq, midifile[track]);
    res += (str.empty() ? "" : str + "\n");
  }

  return res;
}

std::string InfoString(
  smf::MidiFile& midifile,
  std::optional<int> optionalTrack,
  std::optional<std::string> anacrusis,
  std::optional<std::string> optionalTimeSig,
  bool allClefs)
{
  midifile.removeEmpties();
  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();
  const int tracks = midifile.getTrackCount();
  std::string res = "TPQ: " + std::to_string(tpq) +
    " number of tracks: " + std::to_string(tracks) + "\n";

  int anacrusisTicks = GetAnacrusisTicks(anacrusis, tpq);

  // Output just the nominated track, or all of them.
  if (optionalTrack)
  {
    res += InfoStringForOneTrack(midifile, *optionalTrack, anacrusisTicks, optionalTimeSig, allClefs);
  }
  else for (int track = 0; track < tracks; track++)
  {
    res += InfoStringForOneTrack(midifile, track, anacrusisTicks, optionalTimeSig, allClefs);
  }

  return res;
}
}

