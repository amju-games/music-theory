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

}
