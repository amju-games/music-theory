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
  HeroGameRound* r = static_cast<HeroGameRound*>(button->GetUserData());
  Assert(r);
  TheGSHero::Instance()->SetGameRound(r);
  GoTo<TheGSHero>();  
}

GSChooseSong::GSChooseSong()
{
  m_guiFilename = "Gui/gs_choose_song.txt";
}

void SetLevelGui(const HeroGameRound& r, PGuiElement gui)
{
  // We want the whole string localised. So our design should have
  //  a fixed number of levels, say 8 or 10..? It won't be hundreds.
  auto t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("level"));
  Assert(t); 

  // NB convert zero-based level number to one-based string
  //  -- that's for now. We will localise the entire string.
  std::string str = "Level " + std::to_string(r.m_level + 1);
  t->SetText(str); 
}

void SetSongGui(const HeroGameRound& r, PGuiElement gui, int songNum)
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
  t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("song-number"));
  Assert(t); 
  t->SetText(std::to_string(songNum) + "."); 

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
 
  InitGui();
}

void GSChooseSong::InitGui()
{
  // Load GUI for each game round
  auto grm = TheGameRoundManager::Instance();

  // TODO In this situation, we want songs ordered by Level then Round.
  // Conceivably, we might want another sort order in a different 
  //  situation. How about GetGameRound has a sort order param?

  // Make sure the game round csv file is loaded; load only happens
  //  once, right?
  grm->Load();

  auto rootNode = dynamic_cast<GuiComposite*>(
    m_gui->GetElementByName("songs-root"));

  int numSongs = grm->GetNumGameRounds();

  int level = -1; // show level info when level of current song is different

  const float oneSongHeight = 0.9f; // TODO get extent of GUI
  const float levelHeight = 0.5f; 
  float y = 0;
  int songNum = 1;

  for (int i = 0; i < numSongs; i++)
  {
    const auto& gameround = grm->GetGameRound(i);

    // Show level for subsequent songs
    if (gameround.m_level != level)
    {
      level = gameround.m_level;
      songNum = 1;
      // Consider having a different gui file per level
      auto elem = LoadGui("Gui/level-info.txt");
      elem->SetLocalPos(Vec2f(0, y));
      y -= levelHeight;
      SetLevelGui(gameround, elem);
      rootNode->AddChild(elem);
    }

    auto elem = LoadGui("Gui/one-song.txt");
    // Populate text etc in this song GUI
    elem->SetLocalPos(Vec2f(0, y));
    y -= oneSongHeight; // TODO Get extent of song gui
    SetSongGui(gameround, elem, songNum);
    ++songNum;
    rootNode->AddChild(elem);
  }

  // Set scroll bar extents
  auto scroller = dynamic_cast<GuiScroll*>(m_gui->GetElementByName("song-scroller"));
  Assert(scroller);
  scroller->SetExtents(Vec2f(0, -y));
}
}

