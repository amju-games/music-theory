#include <algorithm>
#include <map>
#include <MidiFile.h>
#include "Event.h"
#include "MidiScore.h"

namespace MidiScore
{
enum class TimeSig
{
  TS_4_4,
  TS_3_4,
};

std::string TimeSigString(TimeSig ts)
{
  if (ts == TimeSig::TS_4_4) return "4/4 ";
  if (ts == TimeSig::TS_3_4) return "3/4 ";
  return "";
}

TimeSig GuessTimeSig(int tpq, const Events& events)
{
  int totalDuration = events.back().m_end;
  if (totalDuration % (3 * tpq) == 0) return TimeSig::TS_3_4;
  return TimeSig::TS_4_4;
}

enum class Clef
{
  TREBLE,
  BASS,
};

std::string ClefString(Clef clef)
{
  if (clef == Clef::TREBLE) return "clef-t";
  return "clef-b";
}

Clef GuessClef(const Events& e)
{
  const auto [min, max] = std::minmax_element(e.begin(), e.end(), 
    [](const Event& e1, const Event& e2) { return e1.m_pitch < e2.m_pitch; }
  );
  int min_pitch = min->m_pitch;
//  int max_pitch = max->m_pitch;
  if (min_pitch > 56) return Clef::TREBLE;
  return Clef::BASS;
}

enum class KeySig
{
  KS_SHARP_0,
  KS_SHARP_1,
  KS_SHARP_2,
  KS_SHARP_3,
  KS_SHARP_4,
  KS_SHARP_5,
  KS_SHARP_6,
  KS_SHARP_7,
  KS_FLAT_0,
  KS_FLAT_1,
  KS_FLAT_2,
  KS_FLAT_3,
  KS_FLAT_4,
  KS_FLAT_5,
  KS_FLAT_6,
  KS_FLAT_7,
};

std::string KeySigString(KeySig ks)
{
  return "";
}

KeySig GuessKeySig(const Events& events)
{
  std::map<int, int> pitchDistribution;
  // Make frequency distribution of pitches, all shifted to the same octave
  int numNotes = 0;
  for (const Event& e : events)
  {
    if (e.m_isRest) continue;
 
    numNotes++;
    int p = e.m_pitch % 12;
    pitchDistribution[p]++;
  }
  // Analyse sharps/flats in distribution... prob simplest to hard code
  //  perfect distribution for each key, then check our distribution against
  //  each one, and pick the closest match. 
  const std::vector<std::vector<int>> DISTRIBS = 
  {
    //KS_SHARP_0,
    // c  c# d  d# e  f  f# g  g# a  a# b
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 }, 
    //KS_SHARP_1,
    {  1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 }, // g maj
    //KS_SHARP_2,
    {  0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1 }, // d maj
    //KS_SHARP_3,
    //KS_SHARP_4,
    //KS_SHARP_5,
    //KS_SHARP_6,
    //KS_SHARP_7,
    //KS_FLAT_0,
    // c  db d  eb e  f  gb g  ab a  bb b
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1 }, 
    //KS_FLAT_1,
    {  1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0 }, // f maj
    //KS_FLAT_2,
    //KS_FLAT_3,
    //KS_FLAT_4,
    //KS_FLAT_5,
    //KS_FLAT_6,
    //KS_FLAT_7,
  };
  return KeySig::KS_SHARP_0;
}

void SetTimeVal(int tpq, Event& e)
{
  if (e.m_duration >= 4 * tpq) e.m_timeVal = TimeVal::SEMIBREVE;
  else if (e.m_duration >= 2 * tpq) e.m_timeVal = TimeVal::MINIM;
  else if (e.m_duration >=     tpq) e.m_timeVal = TimeVal::CROTCHET;
  else if (e.m_duration >= tpq / 2) e.m_timeVal = TimeVal::QUAVER;
  else e.m_timeVal = TimeVal::SEMIQUAVER;
}

void AddEventToVec(int tpq, const smf::MidiEvent& mev, Events& events)
{
  if (mev.isNoteOn())
  {
    // Add note event to vec
    Event e;
    e.m_start = mev.tick;
    e.m_duration = mev.getTickDuration();
    e.m_end = e.m_start + e.m_duration;

    SetTimeVal(tpq, e);

    int numBytes = mev.size();
    if (numBytes > 2)
    {   
      e.m_pitch = static_cast<int>(mev[1]);
      e.m_velocity = static_cast<int>(mev[2]);
    }

    events.push_back(e);
  }
}

void OutputNote(int tpq, const smf::MidiEvent& mev)
{
  if (mev.isNoteOn())
  {
    std::cout  << std::dec 
      << "Note: start: " << mev.tick / tpq << " crotchets "
      ////<< " *  dur: " << mev.getDurationInSeconds()  << " seconds"
      << " Dur: " << mev.getTickDuration() / tpq << " crotchets ";

    int numBytes = mev.size();
    if (numBytes > 2)
    {   
      std::cout << "  Pitch: " << static_cast<int>(mev[1]) 
        << "  vel: "  << static_cast<int>(mev[2]) << "\n";
    }   
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

std::string ToString(smf::MidiFile& midifile)
{
  Events events;

  // should we do this? Perhaps we need to do one pass with, 
  //  to get all "verticals", and then one without, to get the different voices.
  // TODO Decide whether or not to join the tracks. If we do, (and if we don't)
  //  -- we need to detect chords.
  midifile.joinTracks(); 

  midifile.doTimeAnalysis();
  midifile.linkNotePairs();

  const int tpq = midifile.getTicksPerQuarterNote();

  int tracks = midifile.getTrackCount();
  
  for (int track = 0; track < tracks; track++) 
  {
    for (int event = 0; event < midifile[track].size(); event++) 
    {
      AddEventToVec(tpq, midifile[track][event], events);
    }   
  }

  if (events.empty())
  {
    return "";
  }

  TimeSig ts = GuessTimeSig(tpq, events);

  Clef clef = GuessClef(events); // from pitch range

  KeySig ks = GuessKeySig(events); // from all pitches

  std::string res;
  res += TimeSigString(ts) + " ";
  res += ClefString(clef) + " ";
  res += KeySigString(ks) + " ";

  // Fill 'gaps' between note events with rests
  // TODO

  // Traverse events. Output time val and pitch when either changes.
  res += OutputEvent(events.front()) + " ";
  for (int i = 1; i < events.size(); i++)
  {
    res += OutputEvent(events[i]);
    //res += OutputEvent(events[i - 1], events[i]);
    res += " ";
  }
  res += "\n";
  return res;
}
}

