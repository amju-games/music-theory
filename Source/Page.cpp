// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <ConfigFile.h>
#include <GameState.h>
#include <GuiElement.h>
#include "GSPages.h"
#include "Page.h"

namespace Amju
{
static void OnHint(GuiElement* hintButton)
{
  TheGSPages::Instance()->OnHint();
}

void Page::SetConfigFile(ConfigFile* cf)
{
  m_config = cf;
}

void Page::OnActive()
{
  // TODO append orientation to gui name
  m_gui = LoadGui("Gui/page_" + m_guiName + ".txt");

  // Adjust position so it sits under the top bar
  m_gui->SetLocalPos(Vec2f(0, -0.2f));

  // Set command for common buttons
  GuiElement* hint = GetElementByName(TheGSPages::Instance()->GetGui(), "hint-button");
  if (hint)
  {
    hint->SetCommand(Amju::OnHint);
  }
}

GuiElement* Page::GetGui()
{
  return m_gui;
}
  
void Page::Draw()
{
  m_gui->Draw();
}

void Page::Update()
{
  m_gui->Update();
}

void Page::SetGameState(GameState* gs)
{
  m_gs = gs;
}

void Page::SetIsLearnMode(bool learn)
{
  m_isLearnMode = learn;
}

bool Page::IsLearnMode() const
{
  return m_isLearnMode;
}
}

