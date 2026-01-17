// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#include <CommandLineArgs.h>
#include "GSShowGui.h"

namespace Amju
{
GSShowGui::GSShowGui()
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

