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

bool GSAnimals::OnKeyEvent(const KeyEvent& ke) 
{
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR)
  {
    if (ke.key == '1')
    {
      static int petIndex = 0;
      GetAnimalController().EatAPet(petIndex % 12);
      ++petIndex;

/*
      Palette pal;
      pal.Load("Image/palette-notes-12-2.png");
      GetAnimalController().AddPetsForGameRound(pal);
*/

/*
      static float z = 0; // quick hack: get a new z plane each time
      // Add a bird or dino: can only move in x
      static bool bird = true;
      GetAnimalController().AddAnimalFixedZ(bird ? "bird" : "cat", z);
      z -= 50.f;
      bird = !bird;
*/
      return true; // consumed
    }
    else if (ke.key == '2')
    {
      GetAnimalController().PetsJump();
/*
      // Add a bird: can move in x-z plane
      GetAnimalController().AddAnimal("bird");
*/

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

