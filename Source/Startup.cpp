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
#ifndef CATCH
// Main function already exists for GTest build
#include <main.h>
#endif
#endif

#ifdef GEKKO
#include <main.h>
#endif

#include <AmjuGLWindowInfo.h>
#include <BassSoundPlayer.h>
#include <CursorManager.h>
#include <Directory.h>
#include <Font.h>
#include <Game.h>
#include <GuiRect.h>
#include <EventPoller.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <SoundManager.h>
#include "Course.h"
#include "GSMainMenu.h"
#include "GSPages.h"
#include "GSPlayNotes.h"
#include "GSShowLineDrawing.h"
#include "GSShowMusicScore.h"
#include "GSTestShowScore.h"
#include "GSTitle.h"
#include "GSTopicEnd.h"
#include "GSUserDrawSave.h"
#include "GuiLineDrawing.h"
#include "GuiMusicKb.h"
#include "GuiMusicScore.h"

#define SOUND_FONT "Grand Piano"


namespace Amju
{
// Create global variable window info 
//Amju::AmjuGLWindowInfo w(320, 550, false, "Amjula");
Amju::AmjuGLWindowInfo w(480, 640, false, "Simplest");

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

  // Musical terms dictionary 
  rm->AddLoader("dictionary", DictionaryLoader);

  // Course 
  rm->AddLoader("course", CourseLoader);

#ifdef AMJU_USE_BASS
  // Set sound player
  SoundManager* sm = TheSoundManager::Instance();
  BassSoundPlayer* bsp = new BassSoundPlayer;
#ifdef USE_BASS_MIDI
  bsp->MidiSetSoundFont((File::GetRoot() + "Sound/" + SOUND_FONT + ".sf2").c_str());
#endif // USE_BASS_MIDI
  sm->SetImpl(bsp); 
#endif

#if defined(WIN32) || defined(MACOSX)
  TheCursorManager::Instance()->Load(Vec2f(0.025f, -0.08f)); // hotspot position
#endif

  // Set image used for rounded rectangles
  GuiRect::SetCornerImage("Image/corner.png");

  GuiLineDrawing::AddToFactory();
  GuiMusicKb::AddToFactory();
  GuiMusicScore::AddToFactory();

//  TheGame::Instance()->SetCurrentState(TheGSPlayNotes::Instance());
  //TheGame::Instance()->SetCurrentState(TheGSShowMusicScore::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShowLineDrawing::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSUserDraw::Instance());

//  TheGame::Instance()->SetCurrentState(TheGSTestShowScore::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSPages::Instance());
//    TheGame::Instance()->SetCurrentState(TheGSUserDrawSave::Instance());
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());

//  TheGame::Instance()->SetCurrentState(TheGSMainMenu::Instance());
  
//  TheGame::Instance()->SetCurrentState(TheGSTopicEnd::Instance());
}
}

