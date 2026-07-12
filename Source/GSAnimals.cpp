#include <CommandLineArgs.h>
#include <Game.h>
#include "AnimalController.h"
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

bool GSAnimals::OnKeyEvent(const KeyEvent& ke) 
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR)
  {
    if (ke.key == '1')
    {
      // Add a bird
      GetAnimalController().AddAnimal("bird");
      return true; // consumed
    }
  }

  return GSBase3d::OnKeyEvent(ke);
}

void GSAnimals::Update()
{
  GSBase3d::Update();
  
  TheGame::Instance()->UpdateGameObjects();
} 
}

