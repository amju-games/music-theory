// * Listen *
// Play midi file using same code path as for game.
// Example:
//   cd Script/Macosx/
//   make -f ListenMakefile
//   ../../Build/Mac/listen Songs/Bach-Air/bach-air.mid


#include <iostream>
#include <AmjuSleep.h>
#include "BassPlayMidi.h"

namespace Amju
{
void SetUpRootDir();
void SetUpSound();
}

int main(int argc, char** argv)
{
  using namespace Amju;

  if (argc < 2)
  {
    std::cout << "Usage: listen <song.mid>\n";
    return 1;
  }

  std::string song(argv[1]);

  std::cout << "Playing " << song << "..\n";

  SetUpRootDir();
  SetUpSound();

  PlayMidiSong(song);
  const auto trackNames = GetPlayingSongTrackNames(); 
  int i = 0;
  for (const auto& t : trackNames)
  {
    std::cout << "Track " << i++ << ":\t" << t << "\n";
  }

  while (true)
  {
    SleepMs(1000);
    std::cout << ".";
    std::flush(std::cout);
  }
  
  return 0;
}

