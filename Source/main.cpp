#ifndef AMJU_IOS

#ifdef WIN32
#if defined(_DEBUG)
#pragma comment(lib, "../../../../../amjulib/Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../amjulib/Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#include <iostream>

#if defined(WIN32) || defined(MACOSX)
#define AMJU_USE_OPENGL
#define AMJU_USE_GLUT
#include <main.h>
#endif

#ifdef GEKKO
#include <main.h>
#endif

#include <AmjuGLWindowInfo.h>
#include <Game.h>
#include <EventPoller.h>
#include <BassSoundPlayer.h>
#include "GSPlayNotes.h"
#include "GSShowLineDrawing.h"
#include "GSShowMusicScore.h"
#include "GuiLineDrawing.h"
#include "GuiMusicScore.h"

namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(480, 640, false, "Simplest");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

void StartUpBeforeCreateWindow()
{
}

void StartUpAfterCreateWindow()
{
  // Set sound player
  SoundManager* sm = TheSoundManager::Instance();
  BassSoundPlayer* bsp = new BassSoundPlayer;
  bsp->MidiSetSoundFont("C:/Users/Jason/projects/music-theory/Assets/Sound/velocity_grand_piano.sf2");
  sm->SetImpl(bsp);

  GuiMusicScore::AddToFactory();
  GuiLineDrawing::AddToFactory();

//  TheGame::Instance()->SetCurrentState(TheGSPlayNotes::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShowMusicScore::Instance());
  TheGame::Instance()->SetCurrentState(TheGSShowLineDrawing::Instance());
}
}

#endif // AMJU_IOS

