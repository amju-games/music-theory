#include <iostream>
#include <DrawRect.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiScroll.h>
#include <GuiText.h>
#include "GSChooseSong.h"
#include "GSConfirmSong.h"
#include "GSHero.h"
#include "GuiMusicScore.h"
#include "HeroGameRound.h"
#include "UserProfile.h"

//#define DEBUG_DRAW_RECTS

namespace Amju
{
static void OnTabStop(GuiElement* scroller, int tabStop)
{
  TheGSChooseSong::Instance()->OnTabStop(tabStop);
}

static void OnQuitButton(GuiElement* button)
{
  TheGame::Instance()->GetState()->GoBack();
}

static void OnSongStart(GuiElement* button)
{
  HeroGameRound* r = static_cast<HeroGameRound*>(button->GetUserData());
  Assert(r);
  TheGSHero::Instance()->SetGameRound(r);

  // Confirm song choice before starting
  GoTo<TheGSConfirmSong>();  
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

void MoveUpMultiLineTitle(GuiTextBase* t)
{
  // Title can span two lines (only two, right?!) Go UP if necessary.
  // So the GUI underneath is not disturbed.
  // TODO This would be better handled by a vertical just setting in GuiTextBase :(
  if (t->GetNumLines() > 1)
  {
    const float ONE_LINE_Y_INC = .3f;
    float yInc = static_cast<float>(t->GetNumLines() - 1) * ONE_LINE_Y_INC;
    auto pos = t->GetLocalPos();
    pos.y += yInc; // +ve y is UP
    t->SetLocalPos(pos);
  }
}

static void SetSongGui(const HeroGameRound& r, PGuiElement gui, int songNum,
  bool isUnlocked, const SongPlayerInfo& spi, bool hasFocus)
{
  auto t = dynamic_cast<GuiTextBase*>(gui->GetElementByName("song-title"));
  Assert(t); // this is all stuff that is fixed at compile time
  t->SetText(r.m_title); 
  MoveUpMultiLineTitle(t);

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
  b->SetHasFocus(hasFocus); 

  // All songs are selectable in debug builds
#ifdef _DEBUG
  b->SetIsEnabled(true); 
#else
  b->SetIsEnabled(isUnlocked);
#endif

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
  GuiScroll::SetTabStopSoundFilename("Sound/wav/click.wav");

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

  const float oneSongWidth = 1.7f;
  const float levelHeight = 0.5f; 
  float x = 0;
  const float TOP_Y = 0.5f;
  float y = 0; // cumulative extent of GUI as we add to it.
  int songNum = 1; // song num in current level; one-based as we display it.
  bool focusHasBeenSet = false; // flag for setting focus on next song. 
  int tabStopForFocusSong = 0; // set tab stop so song with focus is selected.
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
    x += oneSongWidth;
    // TODO player like flag, hi score, completed flag
    const auto& spi = user->GetSongPlayerInfo(gameround.m_name);
    bool hasFocus = false;
    if (isUnlocked && !spi.m_completed && !focusHasBeenSet)
    {
      hasFocus = true;
      focusHasBeenSet = true;
      tabStopForFocusSong = -i; // tab stops go negative, should we change
    }
    SetSongGui(gameround, elem, songNum, isUnlocked, spi, hasFocus);
    ++songNum;
    rootNode->AddChild(elem);
  }

  // Set scroll bar extents
  auto scroller = dynamic_cast<GuiScroll*>(m_gui->GetElementByName("song-scroller"));
  Assert(scroller);

  scroller->SetExtents(Vec2f(x - oneSongWidth, 0));
  scroller->SetTabStopSize(Vec2f(oneSongWidth, 0));
  scroller->SetTabStopCallback(Amju::OnTabStop);
  scroller->SetTabStop(tabStopForFocusSong);
  
  // Set consts so we click each song into place
  scroller->SetStoppingVel(.5f);
  scroller->SetSpeedBumpMult(.25f);
  scroller->SetStoppingDistance(.03f);
}

void GSChooseSong::OnTabStop(int tabStop)
{
  m_lastTabStop = tabStop; // TODO persist this in game config file? Hmm
  std::cout << "Hit tab stop " << tabStop << "\n";
}
}

