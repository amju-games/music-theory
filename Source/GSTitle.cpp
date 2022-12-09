// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include "GSTitle.h"
#include "GSPages.h"

namespace Amju
{
static void OnStart(GuiElement*)
{
  // TODO TEMP TEST
  TheGame::Instance()->SetCurrentState(TheGSPages::Instance());
}

GSTitle::GSTitle()
{
  m_guiFilename = "Gui/gs_title.txt";
}

void GSTitle::OnActive()
{
  GSBase::OnActive();

  // Set button commands
  GuiElement* startButton = GetElementByName(m_gui, "start-button");
  startButton->SetCommand(OnStart);


  // Start animations
}
}


