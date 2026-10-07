// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

#include <Game.h>
#include <GuiButton.h>
#include "AnimalController.h"
#include "GS3dTitle.h" // quit
#include "GSHero.h" // resume
#include "GSPause.h"

namespace Amju
{
static void OnResume(GuiElement*)
{
  TheGame::Instance()->GetState()->GoBack();
}

static void OnQuit(GuiElement*)
{
  // Back to main menu
  // TODO Quit confirm
  TheGSHero::Instance()->CancelResumeTime();

  // Clean up scene/game objects
  GetAnimalController().CleanUp();

  GoTo<TheGS3dTitle>(); // Or GSChooseSong?
}

GSPause::GSPause()
{
  m_guiFilename = "Gui/gs_pause.txt";
}

void GSPause::Update()
{
  UpdateHud();
  GSBase::Update();
}

void GSPause::OnActive()
{
  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "resume-button");
  Assert(elem);
  elem->SetCommand(OnResume);
  auto button = dynamic_cast<GuiButton*>(elem);
  Assert(button);
  button->SetHasFocus(true);

  elem = GetElementByName(m_gui, "quit-button");
  Assert(elem);
  elem->SetCommand(OnQuit);

  InitHud(m_gui);
}
};
