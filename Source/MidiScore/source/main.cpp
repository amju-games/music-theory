// * MidiScore *
// (c) Copyright 2026 Juliet Colman

// MidiScore -- read midi file, output text which can be read by makescore.
// Uses https://github.com/craigsapp/midifile.

#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <MidiFile.h> // 3rd party MidiFile lib
#include "commandline.h"
#include "MidiScore.h"

using namespace std;
using namespace smf;

static const auto TRACK = "--track";
static const auto TIMESIG = "--timesig";
static const auto KEYSIG = "--keysig";
static const auto QUANT = "--quant";
static const auto DEBUG = "--debug";
static const auto BPM = "--bpm";
static const auto ANA = "--ana";
static const auto ALL_CLEFS = "--allclefs";

static const auto USAGE_STRING = 
R"(midiscore - convert midi file to juliet compact notation.
Usage:
  midiscore <midifile> --bpm <f> --timesig <t> [options]

Options:
  --info        Output info about the midi file.
  --track <n>   Output notation for track n only; 
                  Default is all tracks, as separate staves.
                  NB track numbers are zero-based.
  --timesig <t> Set time signature to t, a string. 
                  Format for t is "3/4", "4/4", etc. without quotes.
  --keysig <k>  Set key signature to k, an int. 
                  Positive k is number of sharps; 
                  negative k is number of flats.
  --quant <q>   Quantise starts and durations, to resolution q.
                  q is one of: qqq, qq, q, c, m, sb, sb2, sb4
  --debug       Verbose output for debugging
  --bpm <f>     Set Beats Per Minute tempo: passed to MakeScore.
                  f is a number, can be fractional.
  --ana <a>     Anacrusis: set length of incomplete first bar.
                  a is one of: q, c, m, etc.
  --allclefs    Output alto and tenor clefs as well as bass and treble.
)";

static std::string NowToString()
{
  // https://stackoverflow.com/a/16358111
  // Seriously, how hard should this be.

  auto t = std::time(nullptr);
#ifdef WIN32
  tm timestamp;
  localtime_s(&timestamp, &t);
#else
  auto timestamp = *std::localtime(&t);
#endif

  std::ostringstream oss;
  oss << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S");
  return oss.str();
}

void OutputCommandLine(const commandline& cl)
{
  std::cout << "// Created by MIDISCORE by Juliet, "
    << NowToString()
    << "\n";

  std::cout << "// ";
  const auto& strs = cl.strings();
  for (const auto& s : strs)
  {
    std::cout << s << " ";
  }
  std::cout << "\n";
}

int main(int argc, const char** argv) 
{
  commandline cl(argc, argv);

  if (cl.num_strings() < 2) 
  {
    std::cout << USAGE_STRING;
    return 0;
  }

  OutputCommandLine(cl);

  MidiFile midifile;
  // 0th arg is exe name, 1th arg is midi filename
  std::string filename(argv[1]);

  if (!midifile.read(filename))
  {
    std::cout << "Failed to read midi file \"" << filename << "\"\n";
    return 1;
  }

  // All clefs option
  const bool allClefs = check_flag(cl, ALL_CLEFS);

  // Optional anacrusis bar length
  const auto optionalAna = cl.get_value<std::string>(ANA);

  // Optional track number
  const auto optionalTrack = cl.get_value<int>(TRACK);
  // Check for errors in track number
  if (cl.contains(TRACK))
  {
    if (!optionalTrack)
    {
      std::cout << "Missing or bad track number after " << TRACK << ".\n";
      return 1;
    }
    if (*optionalTrack >= midifile.getNumTracks())
    {
      std::cout << "Track number too big, max is " 
        << (midifile.getNumTracks() - 1) << " (zero-based).\n";
      return 1;
    }
    if (*optionalTrack < 0)
    {
      std::cout << "Negative track number.\n";
      return 1;
    }
    std::cout << "// Output track " << *optionalTrack << " only.\n";
  }

  // If "info" arg exists, output info about the midi file.
  if (check_flag(cl, "--info"))
  {
    // For --info, we can specify the time sig, and get better clef changes.
    const auto optionalTimeSig = cl.get_value(TIMESIG);
    if (cl.contains(TIMESIG) && !optionalTimeSig)
    {
      std::cout << "Missing or bad time signature after " << TIMESIG << "\n";
      return 1;
    }

    std::cout << MidiScore::InfoString(
      midifile, optionalTrack, optionalAna, optionalTimeSig, allClefs);
    return 0;
  }

  // Non-optional time sig
  const auto timeSig = cl.get_value(TIMESIG);
  if (cl.contains(TIMESIG) && !timeSig)
  {
    std::cout << "Missing or bad time signature after " << TIMESIG << "\n";
    return 1;
  }
  if (!timeSig)
  {
    std::cout << "Time sig is required, I can't guess it right now.\n";
    return 1;
  }

  // Optional key sig, overriding guessed key sig.
  const auto keySig = cl.get_value<int>(KEYSIG);

  // Optional quantisation resolution
  const auto quant = cl.get_value<std::string>(QUANT);

  // Debug option
  const bool debug = check_flag(cl, DEBUG);

  // BPM tempo -- passed through to MakeScore, and is required.
  const auto bpm = cl.get_value<float>(BPM);
  if (!bpm)
  {
    std::cout << "No BPM specified! (e.g.: --bpm 120)\n";
    return 1;
  }

  std::cout << MidiScore::ToString(
    midifile, optionalTrack, *timeSig, keySig, quant, debug, *bpm, optionalAna, allClefs);

  return 0;
}
