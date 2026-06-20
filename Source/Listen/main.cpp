// * Listen *
// Play midi file using same code path as for game.
// Example:
//   cd Script/Macosx/
//   make -f ListenMakefile
//   ../../Build/Mac/listen Songs/Bach-Air/bach-air.mid


#include <filesystem>
#include <iostream>
#include <string>
#include <sstream>
#include <set>
#include <vector>
#include <AmjuSleep.h>
#include <File.h>
#include "BassPlayMidi.h"

namespace Amju
{
void SetUpRootDir();
void SetUpSound();
}

// Re-using the parser function from earlier
std::set<int> ParseTrackSelection(const std::string& input) 
{
    std::set<int> selectedTracks;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, ',')) {
        token.erase(0, token.find_first_not_of(" \t\r\n"));
        token.erase(token.find_last_not_of(" \t\r\n") + 1);
        if (token.empty()) continue;

        size_t hyphenPos = token.find('-');
        if (hyphenPos == std::string::npos) {
            try { selectedTracks.insert(std::stoi(token)); } catch (...) {}
        } else {
            try {
                int start = std::stoi(token.substr(0, hyphenPos));
                int end = std::stoi(token.substr(hyphenPos + 1));
                if (start > end) std::swap(start, end);
                for (int i = start; i <= end; ++i) selectedTracks.insert(i);
            } catch (...) {}
        }
    }
    return selectedTracks;
}

bool GetArgs(int argc, char* argv[], std::set<int>& muteTracks, std::set<int>& soloTracks) 
{
    std::string midiFilePath = "";

    // Loop through command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--mute" && i + 1 < argc) {
            muteTracks = ParseTrackSelection(argv[++i]);
        } 
        else if (arg == "--solo" && i + 1 < argc) {
            soloTracks = ParseTrackSelection(argv[++i]);
        } 
        else {
            // Assume any unflagged argument is the path to the MIDI file
            midiFilePath = arg;
        }
    }

    if (midiFilePath.empty()) {
        std::cout << "Usage: listen <file.mid> [--mute tracks] [--solo tracks]\n";
        std::cout << "Example: listen song.mid --mute 0-2,5 --solo 3\n";
        return false;
    }

    // Example verification output
    std::cout << "Tracks to Mute (0-indexed): ";
    for (int t : muteTracks) std::cout << t << " ";
    std::cout << "\nTracks to Solo (0-indexed): ";
    for (int t : soloTracks) std::cout << t << " ";
    std::cout << "\n";

    return true;
}

int main(int argc, char** argv)
{
  using namespace Amju;

  std::set<int> muteTracks;
  std::set<int> soloTracks;
  if (!GetArgs(argc, argv, muteTracks, soloTracks)) return 1;

  std::string song(argv[1]);

  std::cout << "Playing " << song << "..\n";

  // Set cwd to find the sound fonts, which should be in a Sound/ dir,
  //  in the dir where the exe is installed.
  File::SetRoot(std::filesystem::current_path(), "/");
  SetUpSound();

  PlayMidiSong(song);

  // Apply mute/solo flags
  ApplyMuteSoloToPlayingSong(muteTracks, soloTracks);

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

