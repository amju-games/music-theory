// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

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
#include <EventPoller.h>
#include <FileImplGlue.h>
#include <Font.h>
#include <Game.h>
#include <GlueFileMem.h>
#include <GuiRect.h>
#include <Localise.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <ROConfig.h>
#include <SoundManager.h>
#include "Consts.h"
#include "Course.h"
#include "Dictionary.h"
#include "GSFirstUser.h"
#include "GSMainCorridor.h"
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

#ifdef AMJU_IOS
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-iOS.glue"
#define MUSIC_GLUE_FILE "music-iOS.glue"
#endif

#ifdef MACOSX
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-iOS.glue"
#define MUSIC_GLUE_FILE "music-iOS.glue"
#endif

#ifdef WIN32
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-win.glue"
#define MUSIC_GLUE_FILE "music-win.glue"
#endif

namespace Amju
{
// Create global variable window info 
//Amju::AmjuGLWindowInfo w(320, 550, false, "iPhone 5");
//Amju::AmjuGLWindowInfo w(480, 640, false, "Simplest");

Amju::AmjuGLWindowInfo w(1136, 640, false, "Landscape iPhone 5");
//Amju::AmjuGLWindowInfo w(1136/2, 640/2, false, "Landscape iPhone 5 (small)");
// For creating screen shots etc, as big as poss
//Amju::AmjuGLWindowInfo w(2048, 2048, false, "Square");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

static void SetUpRootDir()
{
#ifdef AMJU_IOS
  std::string dir = GetDataDir();
  dir += "/Assets/";
  File::SetRoot(dir, "/");
#endif

#ifdef MACOSX
#ifdef YES_GLUE_FILE
  std::string dir = "/Users/jay/projects/music-theory/Build/CompiledAssets/";
#else
  std::string dir = "/Users/jay/projects/music-theory/Assets/";
#endif
  File::SetRoot(dir, "/");
#endif // MACOSX
}

static void SetUpGlueFile()
{
#ifdef YES_GLUE_FILE

  GlueFileMem* gfm = new GlueFileMem;
  if (FileImplGlue::OpenGlueFile(GLUE_FILE, gfm))
  { 
    std::cout << "Opened glue file " << GLUE_FILE << "\n";
  }
  else
  {
    ReportError("Failed to open data glue file");
  }

  SoundManager* sm = TheSoundManager::Instance();

  GlueFile* pMusicGlueFile = new GlueFileMem;
  if (pMusicGlueFile->OpenGlueFile(MUSIC_GLUE_FILE, true /* read only */))
  {
    sm->SetGlueFile(pMusicGlueFile);
  }
  else
  {
    ReportError("Failed to open music glue file");
  }

#endif // YES_GLUE_FILE

}

void StartUpBeforeCreateWindow()
{
  SetUpRootDir();
  SetROConfigFilename("roconfig.txt");
}

static void SetUpResourceLoaders()
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
}

static void SetUpSound()
{
#ifdef AMJU_USE_BASS

  // Set sound player
  SoundManager* sm = TheSoundManager::Instance();
  BassSoundPlayer* bsp = new BassSoundPlayer;

#ifdef AMJU_USE_BASS_MIDI
  bsp->MidiSetSoundFont((File::GetRoot() + "Sound/" + MIDI_SOUND_FONT + ".sf2").c_str());
#endif // AMJU_USE_BASS_MIDI

  sm->SetImpl(bsp); 
#endif // AMJU_USE_BASS
}

static void SetUpGui()
{
#if defined(WIN32) || defined(MACOSX)
  TheCursorManager::Instance()->Load("Image/hand.png", Vec2f(0.025f, -0.08f)); // hotspot position
#endif

  GuiButton::SetClickFilename(WAV_BUTTON_CLICK);

  // Set image used for rounded rectangles
  GuiRect::SetCornerImage("Image/corner.png");

  GuiLineDrawing::AddToFactory();
  GuiMusicKb::AddToFactory();
  GuiMusicScore::AddToFactory();
}

static void SetInitialState()
{
//  TheGame::Instance()->SetCurrentState(TheGSPlayNotes::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShowMusicScore::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSShowLineDrawing::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSUserDraw::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSUserDrawSave::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSTopicEnd::Instance());
//  TheGame::Instance()->SetCurrentState(TheGSFirstUser::Instance());

//  TheGame::Instance()->SetCurrentState(TheGSTestShowScore::Instance());

//  TheGame::Instance()->SetCurrentState(TheGSMainCorridor::Instance());
  TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
}

void StartUpAfterCreateWindow()
{
  SetUpResourceLoaders();

  SetUpSound();

  SetUpGlueFile();

  // TODO Other languages - preferences
  if (!Localise::LoadStringTable("english.txt"))
  {
    ReportError("No localise string table.");
  }

  SetUpGui();

  // Load the course which this app presents to the user: we only expect there to 
  //  be one instance. We could potentially load this depending on config/user choice.
  Course* course = (Course*)TheResourceManager::Instance()->GetRes("Course/grade1.txt.course");
  SetCourse(course);

  SetInitialState();

}
}

