// * MidiScore *
// (c) Copyright 2026 Juliet Colman

// MidiScore -- read midi file, output makescore text
// Uses https://github.com/craigsapp/midifile

#include <iostream>
#include <iomanip>
#include <MidiFile.h>
#include <Options.h>
#include "MidiScore.h"

using namespace std;
using namespace smf;

int main(int argc, char** argv) 
{
  if (argc == 1) 
  {
    std::cout << R"(midiscore - convert midi file to juliet compact notation.
Usage:
  midiscore <midifile> [--info]
)";
    return 0;
  }

  std::vector<std::string> args(argv + 1, argv + argc);

  MidiFile midifile;
  midifile.read(args[0]); // 0th arg is always filename

  if (args.size() > 1 && args[1] == "--info")
  {
    std::cout << MidiScore::InfoString(midifile);
    return 0;
  }

  std::cout << MidiScore::ToString(midifile);

  return 0;
}
