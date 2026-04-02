#include <GuiComposite.h>
#include <GuiScroll.h>
#include <GuiText.h>
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GuiMusicScore.h"
#include "HeroGameRound.h"

namespace Amju
{
static void OnSongStart(GuiElement* button)
{
  // TODO Set song
  HeroGameRound* r = static_cast<HeroGameRound*>(button->GetUserData());
  Assert(r);
  TheGSHero::Instance()->SetGameRound(r);
  GoTo<TheGSHero>();  
}

GSChooseSong::GSChooseSong()
{
  m_guiFilename = "Gui/gs_choose_song.txt";
}

void SetSongGui(const HeroGameRound& r, PGuiElement gui)
{
  auto t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("song-title"));
  Assert(t); // this is all stuff that is fixed at compile time
  t->SetText(r.m_title); 
  t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("song-subtitle"));
  Assert(t); 
  t->SetText(r.m_subtitle); 
  t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("song-composer"));
  Assert(t); 
  t->SetText(r.m_composer); 

  //auto b = dynamic_cast<GuiButton*>(gui->GetElementByName("start-button"));
  auto b = gui->GetElementByName("song-start-button");
  Assert(b);
  b->SetUserData(const_cast<HeroGameRound*>(&r)); // element in a singleton vector, so ok, riight?
  b->SetCommand(Amju::OnSongStart);

  // Hmm, should we show a preview of the score?
/*
  auto s = dynamic_cast<GuiMusicScore*>(gui->GetElementByName("music-score"));
  const auto& score = r.m_musicScore;
  s->LoadMusicScore(score);
*/
}

void GSChooseSong::OnActive()
{
  GSBase::OnActive();

  // Load GUI for each game round
  auto grm = TheGameRoundManager::Instance();

  grm->Load();

  auto rootNode = dynamic_cast<GuiComposite*>(
    m_gui->GetElementByName("songs-root"));

  int numSongs = grm->GetNumGameRounds();
  const float oneSongHeight = 0.9f;
  for (int i = 0; i < numSongs; i++)
  {
    auto elem = LoadGui("Gui/one-song.txt");
    // Populate text etc in this song GUI
    elem->SetLocalPos(Vec2f(0, -i * 0.9f));
    auto& gameround = grm->GetGameRound(i);
    SetSongGui(gameround, elem);
    rootNode->AddChild(elem);
  }

  // Set scroll bar extents
  auto scroller = dynamic_cast<GuiScroll*>(m_gui->GetElementByName("song-scroller"));
  Assert(scroller);
  scroller->SetExtents(Vec2f(0, (numSongs - 1) * oneSongHeight));
}
}

