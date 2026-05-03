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

void GSVerify::Update() 
{
  static int frame = 0;
//  static std::set<std::string> countInFiles; // s/b member
  auto grm = TheGameRoundManager::Instance();

  if (frame == 0)
  {
    if (!grm->Load())
    {
      std::cout << "Failed to load game rounds csv";
      Assert(0);
    }
  }
  else
  {
    const int n = grm->GetNumGameRounds();
    int i = frame - 1;
    if (i >= n)
    {
      // All loaded. Check count-ins.
      // TODO
      GoTo<TheGS3dTitle>();
      return;
    }
   
    const HeroGameRound& r = grm->GetGameRound(i);

    std::cout << "Verifying " << r.m_name << "\n";
    // TODO Check display strings for localise '@'s
    // Play midi (won't be for long, but will assert if no good).
    PlayMidiSong(r.m_backingTrack);
    
    GuiMusicScore score;
    if (!score.LoadMusicScore(r.m_musicScore))
    {
      std::cout << "Failed to load score: " << r.m_musicScore << "\n";
      Assert(0);
    }
  }

  ++frame;

  // Append a '.' to gui string
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "verify-text"));
  Assert(t);
  std::string str = t->GetText();
  str += ".";
  t->SetText(str);
}
}

