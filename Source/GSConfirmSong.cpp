// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#include <GuiButton.h>
#include <GuiText.h>
#include "GSChooseSong.h"
#include "GSConfirmSong.h"
#include "GSHero.h"
#include "HeroGameRound.h"

namespace Amju
{
static void OnQuit(GuiElement*)
{
  GoTo<TheGSChooseSong>();
}

static void OnStart(GuiElement*)
{
  GoTo<TheGSHero>();
}

GSConfirmSong::GSConfirmSong()
{
  m_guiFilename = "Gui/gs_confirm_song.txt";
}

void GSConfirmSong::OnActive()
{
  GSBase::OnActive();

  GuiElement* elem = GetElementByName(m_gui, "start-button");
  Assert(elem);
  elem->SetCommand(OnStart);
  auto button = dynamic_cast<GuiButton*>(elem);
  Assert(button);
  button->SetHasFocus(true);

  elem = GetElementByName(m_gui, "quit-button");
  Assert(elem);
  elem->SetCommand(OnQuit);

  const auto& r = TheGSHero::Instance()->GetGameRound(); // set in prev state
  auto t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-title"));
  Assert(t); // this is all stuff that is fixed at compile time
  t->SetText(r.m_title); 
  t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-subtitle"));
  Assert(t); 
  t->SetText(r.m_subtitle); 
  t = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "song-composer"));
  Assert(t); 
  t->SetText(r.m_composer); 
}
}

