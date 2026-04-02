#include <SoundManager.h>
#include "GS3dTitle.h"
#include "GSHero.h"
#include "GSHeroWin.h"

namespace Amju
{
static void OnNext(PGuiElement)
{
  GoTo<TheGSHero>();
}

static void OnQuit(PGuiElement)
{
  GoTo<TheGS3dTitle>();
}

GSHeroWin::GSHeroWin()
{
  m_guiFilename = "Gui/gs_hero_win.txt";
}

void GSHeroWin::OnActive()
{
  GSBase::OnActive();

  // Start playing 'won round' music
  auto sm = TheSoundManager::Instance();
  sm->PlaySong("Music/amt-title.it");

  // Set up buttons
  GuiElement* nextButton = GetElementByName(m_gui, "next-button");
  nextButton->SetCommand(OnNext);
  nextButton->SetHasFocus(true);

  GuiElement* quitButton = GetElementByName(m_gui, "quit-button");
  quitButton->SetCommand(OnQuit);

  InitHud();

  // Go back to title after a while?
  // NB We need to clear the message queue if we get a button click
/*
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGS3dTitle>,
    SecondsFromNow(5.f)));
*/
}

}

