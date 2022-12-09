// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include "GSMainMenu.h"
#include "GSTitle.h"

namespace Amju
{
static void OnStart(GuiElement*)
{
  TheGSTitle::Instance()->ScrollUp();
//  TheGame::Instance()->SetCurrentState(TheGSMainMenu::Instance());

  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainMenu>, SecondsFromNow(1.5f)));
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


