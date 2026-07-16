#include <GuiText.h>
#include <SoundManager.h>
#include "GS3dTitle.h" // give up
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GSHeroEnd.h"
#include "GSInitNewRound.h" // retry
#include "HeroGameRound.h"

namespace Amju
{
static void OnRetry(PGuiElement)
{
  GoTo<TheGSInitNewRound>();
}

static void OnQuit(PGuiElement)
{
  GoTo<TheGSChooseSong>();
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

  // Set song title text
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-title"));
  Assert(t);
  t->SetText(TheGSHero::Instance()->GetGameRound().m_title);
  InitHud(m_gui);

  // Go back to title after a while?
  // NB We do clear message queue in OnDeactive so we won't have this 
  //  event hanging around.. right?! 
  const float TIME_UNTIL_WE_GO_BACK_TO_TITLE = 20.f;
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGS3dTitle>,
    SecondsFromNow(TIME_UNTIL_WE_GO_BACK_TO_TITLE)));
}
}

