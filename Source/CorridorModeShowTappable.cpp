// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include "CorridorModeExitTappable.h"
#include "CorridorModeShowTappable.h"
#include "GSMainMenu.h"

namespace Amju
{
namespace
{
void OnDone(GuiElement*)
{
  // If this tappable had not been seen before, add it to the user's NOTEBOOK.
  // Show a message: "You added <topic name> to your notebook!"
  // Play a sound effect. Give hints/other currency as reward?
  // ** Unlock topics if all relevant tappables have been seen? **

  // If we already saw this tappable, just exit out to the corridor again.
  TheGSMainMenu::Instance()->SetMode(CorridorModeExitTappable::ID);
}
} // anon namespace

CorridorModeShowTappable::CorridorModeShowTappable()
{
  m_guiFilename = "Gui/corridor_mode_show_tappable.txt";
}

void CorridorModeShowTappable::Update()
{
  CorridorMode::Update();
}

void CorridorModeShowTappable::OnActive()
{
  CorridorMode::OnActive();

  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "done-button"));
  Assert(button);
  button->SetCommand(OnDone);

  Tappable* tapped = m_gs->GetSelectedTappable();
  Assert(tapped);
  tapped->ActivateGui();
}

void CorridorModeShowTappable::Draw2d()
{
  CorridorMode::Draw2d();
  Tappable* tapped = m_gs->GetSelectedTappable();
  Assert(tapped);
  tapped->DrawGui();
}
}
