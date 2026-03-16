// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <Game.h>
#include <GuiDecAnimation.h>
#include <MessageQueue.h>
#include "GSAbout.h"
#include "PrintGui.h"

namespace Amju
{
static void OnOK(GuiElement* elem)
{
  // TODO go back
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


