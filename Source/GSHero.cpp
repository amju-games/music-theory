// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <CommandLineArgs.h>
#include "Consts.h"
#include "GSHero.h"
#include "GuiMusicScore.h"

namespace Amju
{
GSHero::GSHero()
{
  m_guiFilename = "Gui/gs_hero.txt";
}

void GSHero::OnActive() 
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
