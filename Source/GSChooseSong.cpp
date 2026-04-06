#include <DrawRect.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiScroll.h>
#include <GuiText.h>
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GuiMusicScore.h"
#include "HeroGameRound.h"
#include "UserProfile.h"

//#define DEBUG_DRAW_RECTS

namespace Amju
{
static void OnQuitButton(GuiElement* button)
{
  TheGame::Instance()->GetState()->GoBack();
}

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

void GSChooseSong::Draw2d()
{
#ifdef DEBUG_DRAW_RECTS
  GSShowGui::Draw2d();
#else
  GSBase::Draw2d();
#endif
}

static void SetLevelGui(const HeroGameRound& r, PGuiElement gui)
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

static void SetSongGui(const HeroGameRound& r, PGuiElement gui, int songNum,
  bool isUnlocked)
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

  auto elem = gui->GetElementByName("song-start-button");
  auto b = dynamic_cast<GuiButton*>(elem);
  Assert(b);
  b->SetUserData(const_cast<HeroGameRound*>(&r)); // element in a singleton vector, so ok, riight?
  b->SetCommand(Amju::OnSongStart);
  b->SetIsEnabled(isUnlocked);

  // Hmm, should we show a preview of the score?
/*
  auto s = dynamic_cast<GuiMusicScore*>(gui->GetElementByName("music-score"));
  const auto& score = r.m_musicScore;
  s->LoadMusicScore(score);
*/
}

void GSChooseSong::OnActive()
{
  GSShowGui::OnActive();
  InitGui();
}

void GSChooseSong::InitGui()
{
  InitQuitButton();
  InitScrollingGui();
}

void GSChooseSong::InitQuitButton()
{
  GuiElement* elem = GetElementByName(m_gui, "quit-button");
  Assert(elem); // include quit-button.txt!
  elem->SetCommand(Amju::OnQuitButton);
}

void GSChooseSong::InitScrollingGui()
{
  auto grm = TheGameRoundManager::Instance();
  // Make sure the game round csv file is loaded; load only happens
  //  once, right?
  grm->Load();

  // Get the user profile, i.e. saved game state for current player.
  auto user = GetUserProfile();

  auto unlocked = CalcUnlockedSongNames(grm, user);

  // TODO In this situation, we want songs ordered by Level then Round.
  // Conceivably, we might want another sort order in a different 
  //  situation. How about GetGameRound has a sort order param?

  auto rootNode = dynamic_cast<GuiComposite*>(
    m_gui->GetElementByName("songs-root"));

  int numSongs = grm->GetNumGameRounds();

  int level = -1; // show level info when level of current song is different

  const float oneSongHeight = 0.9f; // TODO get extent of GUI
  const float levelHeight = 0.5f; 
  float x = 0;
  const float TOP_Y = 0.5f;
  float y = 0; // cumulative extent of GUI as we add to it.
  int songNum = 1; // song num in current level; one-based as we display it.

  for (int i = 0; i < numSongs; i++)
  {
    const auto& gameround = grm->GetGameRound(i);

    // Show level for subsequent songs
    if (gameround.m_level != level)
    {
      level = gameround.m_level;
      // New level; reset counters
      songNum = 1;

      // Consider having a different gui file per level
      auto elem = LoadGui("Gui/level-info.txt");
      // Set pos, init gui and add to root
      y = TOP_Y;
      elem->SetLocalPos(Vec2f(x, y));
      y -= levelHeight;
      SetLevelGui(gameround, elem);
      rootNode->AddChild(elem);
    }
     
    bool isUnlocked = unlocked.contains(gameround.m_name);

    auto elem = LoadGui(isUnlocked ? "Gui/one-song.txt" : "gui/one-song-locked.txt");
    // Populate text etc in this song GUI
    elem->SetLocalPos(Vec2f(x, y));
    //y -= oneSongHeight; // TODO Get extent of song gui
    x += 1.8f; // TODO TEMP TEST
    // TODO player like flag, hi score, completed flag
    SetSongGui(gameround, elem, songNum, isUnlocked);
    ++songNum;
    rootNode->AddChild(elem);
  }

  // Set scroll bar extents
  auto scroller = dynamic_cast<GuiScroll*>(m_gui->GetElementByName("song-scroller"));
  Assert(scroller);

  // Set scroll region so it just covers the GUI we made above.
  auto size = rootNode->CalcRect().GetSize();
#ifdef _DEBUG
std::cout << "Scroll region size: " << size.x << ", " << size.y << "\n";
#endif
  scroller->SetExtents(size);
}
}

