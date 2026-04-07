#include <GuiText.h>
#include <SoundManager.h>
#include "GS3dTitle.h"
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GSHeroWin.h"
#include "HeroGameRound.h"

namespace Amju
{
static void OnNext(PGuiElement)
{
  // Next button takes us back to the Choose Song page --
  //  if a new level is unlocked, we should give it some razzmatazz
  GoTo<TheGSChooseSong>();
}

GSHeroWin::GSHeroWin()
{
  m_guiFilename = "Gui/gs_hero_win.txt";
}

void GSHeroWin::OnActive()
{
  GSBase::OnActive();

  // Start playing 'won round' music
//  auto sm = TheSoundManager::Instance();
//  sm->PlaySong("Music/amt-title.it");

  // Set up buttons
  GuiElement* nextButton = GetElementByName(m_gui, "next-button");
  nextButton->SetCommand(OnNext);
  nextButton->SetHasFocus(true);

  // Set song title text
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-title"));
  Assert(t);
  t->SetText(TheGSHero::Instance()->GetGameRound().m_title);

  InitHud();
}
}

