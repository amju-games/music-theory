#include <GuiText.h>
#include "BassPlayMidi.h"
#include "GS3dTitle.h"
#include "GSVerify.h"
#include "GuiMusicScore.h"
#include "HeroGameRound.h"

namespace Amju
{
GSVerify::GSVerify()
{
  m_guiFilename = "Gui/gs_verify.txt";
}

// Break verification up into steps. Each step verifies something 
//  different, and is handled by a different function.
// To add a new step:
//   - add a new enum to VerifyStep
//   - add case to switch in Update
//   - write new function; return true when step is complete.

enum VerifyStep
{
  LOAD_SONGS_CSV,
  LOAD_MIDI_FILES,

  DONE // final step
};

static bool VerifyLoadSongsCsv([[maybe_unused]]int frame)
{
  auto grm = TheGameRoundManager::Instance();
  if (!grm->Load())
  {
    std::cout << "Failed to load game rounds csv";
    Assert(0);
  }
  return true;  
}

static bool VerifyLoadSong(int i)
{
  auto grm = TheGameRoundManager::Instance();
  if (i == grm->GetNumGameRounds()) return true; // finished
  const HeroGameRound& r = grm->GetGameRound(i);

  std::cout << "Verifying " << r.m_name << "\n";
  // TODO Check display strings for localise '@'s
  // Play midi (won't be for long, but will assert if no good).
  PlayMidiSong(r.m_backingTrack);
  std::cout << "  ..midi loaded ok\n";
 
  GuiMusicScore score;
  if (!score.LoadMusicScore(r.m_musicScore))
  {
    std::cout << "Failed to load score: " << r.m_musicScore << "\n";
    Assert(0);
  }
  std::cout << "  ..score loaded ok\n";

  // We will load the same count-ins multiple times but it's simpler.
  PlayMidiSong(r.m_countIn); 
  std::cout << "  ..count-in midi loaded ok\n";

  return false; // not finished
}

void GSVerify::Update() 
{
  static VerifyStep step = {};

  static int frame = 0;

  bool finished = false;
  switch (step)
  {
  case LOAD_SONGS_CSV:
    finished = VerifyLoadSongsCsv(frame);
    break;
  case LOAD_MIDI_FILES:
    finished = VerifyLoadSong(frame);
    break;
 
  // ...

  case DONE:
    GoTo<TheGS3dTitle>();
    return;
  }

  ++frame;
  if (finished)
  {
    frame = 0;
    step = static_cast<VerifyStep>(static_cast<int>(step) + 1);
  }

  // Append a '.' to gui string
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "verify-text"));
  Assert(t);
  std::string str = t->GetText();
  str += ".";
  t->SetText(str);
}
}

