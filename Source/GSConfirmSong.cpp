// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#include <GuiButton.h>
#include <GuiText.h>
#include "AnimalController.h"
#include "BassPlayMidi.h"
#include "GSChooseSong.h" // go back
#include "GSConfirmSong.h"
#include "GSHero.h" // to get game round - TODO Move
#include "GSInitNewRound.h" // go forward
#include "HeroGameRound.h"

namespace Amju
{
static void OnQuit(GuiElement*)
{
  // Go back to Choose menu state
  GoTo<TheGSChooseSong>();
}

static void OnStart(GuiElement*)
{
  // Initialise the new game round.
  GoTo<TheGSInitNewRound>();
}

GSConfirmSong::GSConfirmSong()
{
  m_guiFilename = "Gui/gs_confirm_song.txt";
}

void GSConfirmSong::OnActive()
{
  GSBase::OnActive();

  InitButtons();
  InitText();

  // Preview song: play track and show score. Maybe we want to do this
  //  in a 'preview' state?
//  InitScore();
//  StartSong();
}

void GSConfirmSong::InitScore()
{
  auto score = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "the-score"));
  Assert(score);
  const auto& r = TheGSHero::Instance()->GetGameRound(); // set in prev state
  score->LoadMusicScore(r.m_musicScore);
}

void GSConfirmSong::InitButtons()
{
  GuiElement* elem = GetElementByName(m_gui, "start-button");
  Assert(elem);
  elem->SetCommand(OnStart);
  auto button = dynamic_cast<GuiButton*>(elem);
  Assert(button);
  button->SetHasFocus(true);

  elem = GetElementByName(m_gui, "quit-button");
  Assert(elem);
  elem->SetCommand(OnQuit);
}

void GSConfirmSong::InitText()
{
  const auto& r = TheGSHero::Instance()->GetGameRound();
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-title"));
  Assert(t); // this is all stuff that is fixed at compile time
  t->SetText(r.m_title); 
  MoveUpMultiLineTitle(t);

  t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-subtitle"));
  Assert(t); 
  t->SetText(r.m_subtitle); 
  t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-composer"));
  Assert(t); 
  t->SetText(r.m_composer); 
}

void GSConfirmSong::StartSong()
{
  // Play the backing track, with player melody un-muted
  bool noMute = false;
  float startTime = 0; // TODO Get from game round
  const auto& r = TheGSHero::Instance()->GetGameRound(); 
  PlayMidiSong(r.m_backingTrack, startTime, noMute);
}
}

