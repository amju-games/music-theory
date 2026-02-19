// * MidiScore *
// (c) Copyright 2026 Juliet Colman

// MidiScore -- read midi file, output text which can be read by makescore.
// Uses https://github.com/craigsapp/midifile.

#include <iostream>
#include <MidiFile.h> // 3rd party MidiFile lib
#include "commandline.h"
#include "MidiScore.h"

using namespace std;
using namespace smf;

static const auto TRACK = "--track";
static const auto TIMESIG = "--timesig";
static const auto KEYSIG = "--keysig";
static const auto QUANT = "--quant";

static const auto USAGE_STRING = 
R"(midiscore - convert midi file to juliet compact notation.
Usage:
  midiscore <midifile> [options]

Options:
  --info        Output info about the midi file.
  --track <n>   Output notation for track n only; 
                  Default is all tracks, as separate staves.
                  NB track numbers are zero-based.
  --timesig <t> Set time signature to t, a string. 
                  Format is "3/4", "4/4", etc. without quotes.
  --keysig <k>  Set key signature to k, an int. 
                  Positive k is number of sharps; 
                  negative k is number of flats.
  --quant <q>   Quantise starts and durations, to resolution q.
                  Q is one of: qqq, qq, q, c, m, sb, sb2, sb4
)";

int main(int argc, const char** argv) 
{
  commandline cl(argc, argv);

  if (cl.num_strings() < 2) 
  {
    std::cout << USAGE_STRING;
    return 0;
  }

  MidiFile midifile;
  // 0th arg is exe name, 1th arg is midi filename
  midifile.read(cl.get(1).value()); 

  // If "info" arg exists, output info about the midi file.
  if (check_flag(cl, "--info"))
  {
    std::cout << MidiScore::InfoString(midifile);
    return 0;
  }

  // Optional track number
  const auto track = cl.get_value<int>(TRACK);
  // Check for errors in track number
  if (cl.contains(TRACK))
  {
    if (!track)
    {
      std::cout << "Missing or bad track number after " << TRACK << ".\n";
      return 1;
    }
    if (*track >= midifile.getNumTracks())
    {
      std::cout << "Track number too big, max is " 
        << (midifile.getNumTracks() - 1) << " (zero-based).\n";
      return 1;
    }
    if (*track < 0)
    {
      std::cout << "Negative track number.\n";
      return 1;
    }
    std::cout << "// Output track " << *track << " only.\n";
  }

  // Optional time sig, overriding guessed time sig.
  const auto timeSig = cl.get_value(TIMESIG);
  if (cl.contains(TIMESIG) && !timeSig)
  {
    std::cout << "Missing or bad time signature after " << TIMESIG << "\n";
    return 1;
  }

  // Optional key sig, overriding guessed key sig.
  const auto keySig = cl.get_value<int>(KEYSIG);

  // Optional quantisation resolution
  const auto quant = cl.get_value<std::string>(QUANT);

  std::cout << MidiScore::ToString(midifile, track, timeSig, keySig, quant);

  return 0;
}
