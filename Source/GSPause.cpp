// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include "GSMainCorridor.h"
#include "GSPause.h"

namespace Amju
{
static void OnResume(GuiElement*)
{
  TheGame::Instance()->GetState()->GoBack();
}

static void OnQuitTopic(GuiElement*)
{
  // Back to main menu
  // TODO Quit confirm
  TheGame::Instance()->SetCurrentState(TheGSMainCorridor::Instance());
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

  elem = GetElementByName(m_gui, "quit-topic-button");
  Assert(elem);
  elem->SetCommand(OnQuitTopic);
}
};
