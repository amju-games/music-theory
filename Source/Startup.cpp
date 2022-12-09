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
#ifndef GTEST
// Main function already exists for GTest build
#include <main.h>
#endif
#endif

#ifdef GEKKO
#include <main.h>
#endif

#include <AmjuGLWindowInfo.h>
#include <BassSoundPlayer.h>
#include <Directory.h>
#include <Font.h>
#include <Game.h>
#include <EventPoller.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <SoundManager.h>
#include "GSPlayNotes.h"
#include "GSShowLineDrawing.h"
#include "GSShowMusicScore.h"
#include "GSTestShowScore.h"
#include "GSUserDraw.h"
#include "GuiLineDrawing.h"
#include "GuiMusicKb.h"
#include "GuiMusicScore.h"

#define SOUND_FONT "Grand Piano"


namespace Amju
{
// Create global variable window info 
Amju::AmjuGLWindowInfo w(480, 640, false, "Simplest");
//Amju::AmjuGLWindowInfo w(640, 480, false, "Simplest");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

void StartUpBeforeCreateWindow()
{
#ifdef AMJU_IOS
  std::string dir = GetDataDir();
  dir += "/Assets/";
  File::SetRoot(dir, "/");
#endif

#ifdef MACOSX
  std::string dir = "/Users/jay/projects/music-theory/Assets/";
  File::SetRoot(dir, "/");
#endif

}

void StartUpAfterCreateWindow()
{
  // Add resource loaders
  ResourceManager* rm = TheResourceManager::Instance();
  rm->AddLoader("font", FontLoader);

#ifdef YES_BINARY_OBJ_FILES
  rm->AddLoader("obj", BinaryObjLoader);
#else
  rm->AddLoader("obj", TextObjLoader);
#endif

#ifdef AMJU_USE_BASS
  // Set sound player
  SoundManager* sm = TheSoundManager::Instance();
  BassSoundPlayer* bsp = new BassSoundPlayer;
#ifdef USE_BASS_MIDI
  bsp->MidiSetSoundFont((File::GetRoot() + "Sound/" + SOUND_FONT + ".sf2").c_str());
#endif // USE_BASS_MIDI
  sm->SetImpl(bsp); 
#endif

  GuiLineDrawing::AddToFactory();
  GuiMusicKb::AddToFactory();
  GuiMusicScore::AddToFactory();

//  TheGame::Instance()->SetCurrentState(TheGSPlayNotes::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSShowMusicScore::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShowLineDrawing::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSUserDraw::Instance());

  TheGame::Instance()->SetCurrentState(TheGSTestShowScore::Instance());
  
}
}

