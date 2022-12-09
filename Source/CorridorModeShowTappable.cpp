// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiButton.h>
#include "CorridorModeExitTappable.h"
#include "CorridorModeShowTappable.h"
#include "GSMainCorridor.h"

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
  TheGSMainCorridor::Instance()->SetMode(CorridorModeExitTappable::ID);
}
} // anon namespace

const int CorridorModeShowTappable::ID = 3;

CorridorModeShowTappable::CorridorModeShowTappable()
{
  m_guiFilename = "Gui/corridor_mode_show_tappable.txt";
}

void CorridorModeShowTappable::OnActive()
{
  CorridorMode::OnActive();

  GuiButton* button = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "done-button"));
  Assert(button);
  button->SetCommand(OnDone);

  m_tapped = m_gs->GetSelectedTappable();
  Assert(m_tapped);
  m_tapped->ActivateGui();
}

void CorridorModeShowTappable::OnDeactive()
{
  CorridorMode::OnDeactive();
  m_tapped->DeactivateGui();
  m_tapped = nullptr;
}

void CorridorModeShowTappable::Draw2d()
{
  CorridorMode::Draw2d();

  m_tapped->DrawGui();
}

void CorridorModeShowTappable::Update()
{
  CorridorMode::Update();

  m_tapped->UpdateGui();
}
}
