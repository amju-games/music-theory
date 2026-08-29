#include "GSCredits.h"

namespace Amju
{
static void OnBackButton(GuiElement* elem)
{
  TheGSCredits::Instance()->GoBack();
}

void OnCreditsButton(GuiElement* elem)
{
  // TODO Play button song
  //PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music

  TheGSCredits::Instance()->SetPrevState(TheGame::Instance()->GetState());
  TheMessageQueue::Instance()->Clear(); // kill timeline events
  GoTo<TheGSCredits>();
}

GSCredits::GSCredits()
{
  m_guiFilename = "Gui/gs_credits.txt";
  m_sceneFilename = "Scene/credits-scene.txt";
}

void GSCredits::OnActive()
{
  GSBase3d::OnActive(); // Does not automatically load 3d scene.
  Reload3d();

  // Start playing music  -- timeline event?

  // Set up back button
  // TODO

  // Quit button (red, top right)
  auto backButton = GetElementByName(m_gui, "quit-button");
  backButton->SetCommand(OnBackButton);
//  backButton->SetHasFocus(true);
}
}

