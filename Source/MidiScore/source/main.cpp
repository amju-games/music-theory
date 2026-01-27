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

int main(int argc, char** argv) {
  Options options;
  options.process(argc, argv);
  MidiFile midifile;
  if (options.getArgCount() == 0) midifile.read(cin);
  else midifile.read(options.getArg(1));

  std::cout << MidiScore::ToString(midifile);

  return 0;
}
