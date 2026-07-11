#include <CommandLineArgs.h>
#include "GSAnimals.h"

namespace Amju
{
GSAnimals::GSAnimals()
{
  const auto& args = GetCommandLineArgs();
  
  // Get the final command line arg string and use it as scene filename
  const auto& strings = args.GetArgs();
  if (strings.size() > 2)
  {   
    m_sceneFilename = strings.back();
  }   
  else
  {
    // Fallback scene file
    m_sceneFilename = "Scene/animals-persp.txt";
  }

  m_guiFilename = "Gui/no_gui.txt";
}
}

