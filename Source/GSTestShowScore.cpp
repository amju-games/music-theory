// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <CommandLineArgs.h>
#include "Consts.h"
#include "GSTestShowScore.h"
#include "GuiMusicScore.h"

namespace Amju
{
GSTestShowScore::GSTestShowScore()
{
  m_guiFilename = "Gui/gs_test_show_score.txt";
}

void GSTestShowScore::OnActive() 
{
  const auto& args = GetCommandLineArgs();
  if (args.Contains(VIEWSCORE))
  {
    // Get the final command line arg string and use it as gui filename
    const auto& strings = args.GetArgs();
    if (!strings.empty())
    {
      m_guiFilename = strings.back();
    }
  }

  GSBase::OnActive();  
}
}
