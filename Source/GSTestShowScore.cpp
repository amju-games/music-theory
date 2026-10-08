// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

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
  // Get the final command line arg string and use it as gui filename
  const auto& strings = args.GetArgs();
  if (!strings.empty())
  {
    m_guiFilename = strings.back();
  }

  GSBase::OnActive();  
}
}
