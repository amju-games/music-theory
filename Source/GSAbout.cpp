// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include "GSAbout.h"
#include "GSMainMenu.h"
#include "PrintGui.h"

namespace Amju
{
static void OnOK(GuiElement* elem)
{
  TheGSAbout::Instance()->HideButtons()->ScrollUp();
  TheMessageQueue::Instance()->Add(new FuncMsg(GoTo<TheGSMainMenu>, SecondsFromNow(0.5f)));
  // TODO Also stop underground anims?
}

GSAbout::GSAbout()
{
  m_guiFilename = "Gui/gs_about.txt";
}

void GSAbout::OnActive()
{
  GSBase::OnActive();

  //PrintGui(m_gui);

  // Set button commands
  GuiElement* startButton = GetElementByName(m_gui, "ok-button");
  startButton->SetCommand(OnOK);
}

}


