// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include "GSPause.h"

namespace Amju
{
static void OnResume(GuiElement*)
{
  // Back to main menu
  TheGame::Instance()->GetState()->GoBack();
}

GSPause::GSPause()
{
  m_guiFilename = "Gui/gs_pause.txt";
}

void GSPause::OnActive()
{
  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "resume-button");
  Assert(elem);
  elem->SetCommand(OnResume);
}
};
