#include <SoundManager.h>
#include "GS3dTitle.h"
#include "GSHero.h"
#include "GSHeroEnd.h"

namespace Amju
{
static void OnRetry(PGuiElement)
{
  GoTo<TheGSHero>();
}

static void OnQuit(PGuiElement)
{
  GoTo<TheGS3dTitle>();
}

GSHeroEnd::GSHeroEnd()
{
  m_guiFilename = "Gui/gs_hero_end.txt";
}

void GSHeroEnd::Update()
{
  UpdateHud();
  GSBase::Update();
}

void GSHeroEnd::OnActive()
{
  GSBase::OnActive();

  // Start playing end of round music
  auto sm = TheSoundManager::Instance();
  sm->PlaySong("Music/amt-title.it");

  // Set up buttons
  GuiElement* retryButton = GetElementByName(m_gui, "retry-button");
  retryButton->SetCommand(OnRetry);
  retryButton->SetHasFocus(true);

  GuiElement* quitButton = GetElementByName(m_gui, "quit-button");
  quitButton->SetCommand(OnQuit);

  InitHud(m_gui);

  // Go back to title after a while?
  // NB We need to clear the message queue if we get a button click
/*
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGS3dTitle>,
    SecondsFromNow(5.f)));
*/
}

}

