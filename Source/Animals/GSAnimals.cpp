#include <CommandLineArgs.h>
#include <Game.h>
#include "AnimalController.h"
#include "GSAnimals.h"
#include "KeyInputHandler.h"

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

void GSAnimals::OnDeactive()
{
  GSBase3d::OnDeactive();
  GetAnimalController().CleanUp();
}

void GSAnimals::OnActive()
{
  GSBase3d::OnActive();

  Reload3d(); // we do want to do this here, but it's not done automatically.
  GetAnimalController().Init();

std::cout << "Num game objects is now " 
  << TheGame::Instance()->GetGameObjects()->size() 
  << ".\n";
}

KeyInputHandler& GSAnimals::AddKeyInputHandlers()
{
  auto& kih = GSBase3d::AddKeyInputHandlers();

  bool added = true;

#ifdef _DEBUG
  added = kih.AddHandler(MakeKeyEvent('1'), 
    [](const KeyEvent&)->bool 
    {
      static int petIndex = 0;
      GetAnimalController().EatAPet(petIndex % 12);
      ++petIndex;
      return true;
    },
    "Eat a pet");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('2'), 
    [](const KeyEvent&)->bool 
    {
      GetAnimalController().PetsJump();
      return true;
    },
    "Pets jump");
  Assert(added);

#endif

  return kih;
}

void GSAnimals::Update()
{
  GSBase3d::Update();
  
  TheGame::Instance()->UpdateGameObjects();
} 
}

