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
// Main function already exists for Catch (unit test) build
#include <main.h>
#endif
#endif

#ifdef GEKKO
#include <main.h>
#endif

#include <AmjuGLWindowInfo.h>
#include <AmjuRand.h>
#include <BassSoundPlayer.h>
#include <CursorManager.h>
#include <Directory.h>
#include <EventPoller.h>
#include <FileImplGlue.h>
#include <Font.h>
#include <Game.h>
#include <GlueFileMem.h>
#include <GuiFactory.h>
#include <GuiRect.h>
#include <iOSUtils.h>
#include <Localise.h>
#include <ObjMesh.h>
#include <ResourceManager.h>
#include <ROConfig.h>
#include <SceneNodeFactory.h>
#include <SoundManager.h>
#include "ColourPicker.h"
#include "ComposerList.h"
#include "Consts.h"
#include "Course.h"
#include "Dictionary.h"
#include "GSAvatarMod.h"
#include "GSCopyAssets.h"
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
#include "Gui3dScene.h"
#include "GuiAvatar.h"
#include "GuiLineDrawing.h"
#include "GuiMusicKb.h"
#include "GuiMusicScore.h"
#include "NetSend.h"
#include "ParticleFx.h"
#include "SceneNodeGui.h"

#ifdef AMJU_IOS
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-iOS.glue"
#define MUSIC_GLUE_FILE "music-iOS.glue"
#endif

#ifdef MACOSX
//#define YES_GLUE_FILE
//#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-iOS.glue"
#define MUSIC_GLUE_FILE "music-iOS.glue"
#endif

#ifdef WIN32

#ifdef _DEBUG
// MSVC memory debugging
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>  
#endif

#ifdef NDEBUG
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-win.glue"
#define MUSIC_GLUE_FILE "music-win.glue"
#endif // NDEBUG
#endif

// Probably just for now
#define YES_FPS_COUNTER

namespace Amju
{
// Create global variable window info 
//Amju::AmjuGLWindowInfo w(320, 550, false, "iPhone 5");
//Amju::AmjuGLWindowInfo w(480, 640, false, "Simplest");

//Amju::AmjuGLWindowInfo w(1136, 640, false, "Landscape iPhone 5");
Amju::AmjuGLWindowInfo w(1136/2, 640/2, false, "Landscape iPhone 5 (small)");
// For creating screen shots etc, as big as poss
//Amju::AmjuGLWindowInfo w(2048, 2048, false, "Square");

void ReportError(const std::string& str)
{
  std::cout << str << "\n";
}

namespace
{
bool MyFileExists(const std::string& filename)
{
#ifdef YES_GLUE_FILE
  GlueFile* gf = FileImplGlue::GetGlueFile();
  Assert(gf);
  return gf->FileExists(filename);
#else // YES_GLUE_FILE
#ifdef MACOSX
  return FileExists(File::GetRoot() + filename);
#else // MACOSX
  return FileExists(filename);
#endif // MACOSX
#endif // YES_GLUE_FILE
}

// Filename for the writable game config file, not the read-only config.
std::string ConfigFilename()
{
  std::string filename = GetSaveDir(APPNAME) + "config.txt";

#ifdef _DEBUG
  std::cout << "Config file: " << filename << "\n";
#endif

  return filename;
}

void SetUpRootDir()
{
#ifdef AMJU_IOS
  std::string dir = GetDataDir();

#ifndef YES_GLUE_FILE
  dir += "/Assets/";
#endif // YES_GLUE_FILE

  File::SetRoot(dir, "/");
#endif

#ifdef MACOSX

// TODO Fix these harcoded paths!
#ifdef YES_GLUE_FILE
  std::string dir = "/Users/juliet/repos/music-theory/Build/CompiledAssets/";
#else
  std::string dir = "/Users/juliet/repos/music-theory/Assets/";
#endif

  File::SetRoot(dir, "/");
#endif // MACOSX
}

void SetUpGlueFile()
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
    std::cout << "Set music glue file " << MUSIC_GLUE_FILE << "\n";
  }
  else
  {
    ReportError("Failed to open music glue file");
  }

#endif // YES_GLUE_FILE
}

// Send info about this device to the server.
void SendDeviceInfo(bool isFirstTime)
{
  if (isFirstTime)
  {
    std::cout << "Sending device info, first time ever.\n";
    NetSendDeviceInfoFirstRunEver();
  }
  else
  {
    std::cout << "Sending updated device info, NOT first time ever.\n";
    NetSendUpdateDeviceInfo();
  }
}

// Load the Game Config File, which is writable, so used to persist 
//  game-wide info.
void LoadWritableConfig()
{
  GameConfigFile* gcf = TheGameConfigFile::Instance();
  std::string filename = ConfigFilename();
  gcf->SetFilePath(filename);

  bool isFirstTime = true;
  if (FileExists(filename))
  {
    std::cout << "Game config file exists: " << filename << "\n";
    if (gcf->Load())
    {
      std::cout << "Loaded game config file OK: " << filename << "\n";
      // Send updated device info if anything has changed since we last sent.

      if (gcf->Exists(DEVICE_ID))
      {
        std::cout << "Device ID exists in config file: " <<
          gcf->GetValue(DEVICE_ID) << "\n";
        isFirstTime = false; // we have run before!
      }
    }
  }

  SendDeviceInfo(isFirstTime);
}
} // anon namespace

void StartUpBeforeCreateWindow()
{
#if defined(WIN32) && defined(_DEBUG)
  // Set up MSVC mem leak reporting
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  Amju::Randomise();
  
  SetUpRootDir();
  
  SetROConfigFilename(GetSaveDir(APPNAME) + "/roconfig.txt");

  LoadWritableConfig();
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

  // To load dictionaries for tests 
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

template <class T>
static void AddToGuiFactory()
{
  TheGuiFactory::Instance()->Add(T::NAME, []()->GuiElement* { return new T; });
}

static void SetUpGui()
{
#if defined(WIN32) || defined(MACOSX)
  TheCursorManager::Instance()->Load("Image/hand.png", Vec2f(0.025f, -0.08f)); // hotspot position
#endif

  GuiButton::SetClickFilename(WAV_BUTTON_CLICK);

  // Set image used for rounded rectangles
  GuiRect::SetCornerImage("Image/circle.png");

  // Add game-specific types to Gui factory
  AddToGuiFactory<ColourPicker>();
  AddToGuiFactory<Gui3dScene>();
  AddToGuiFactory<GuiAvatar>();
  AddToGuiFactory<GuiLineDrawing>();
  AddToGuiFactory<GuiMusicKb>();
  AddToGuiFactory<GuiMusicScore>();

  //Add game-specific types to Scene node factory
  ParticleFx::AddToFactory();

  TheSceneNodeFactory::Instance()->Add(
    SceneNodeGui::NAME, 
    []()->SceneNode* { return new SceneNodeGui; } 
  );
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
//  TheGame::Instance()->SetCurrentState(TheGSAvatarMod::Instance());
}

static void LoadStringTableForPreferredLanguage()
{
  std::string language = "en-GB";
  
#ifdef AMJU_IOS
  language = GetDevicePreferredLanguage();
  std::cout << "Preferred language: " << language << "\n";
  if (language.empty())
  {
    // TODO We should send this info back to Amju HQ
    // AMJU_TRACKING
    std::cout << "No preferred language found! Report this interesting finding!\n";
    language = "en-GB";
  }
#endif // AMJU_IOS
  
  // Use the preferred language code to load the appropriate string table
  std::string stringTableFile = language + ".txt";
  if (MyFileExists(stringTableFile))
  {
    if (Localise::LoadStringTable(stringTableFile))
    {
      std::cout << "Loaded preferred string table file " << stringTableFile << "\n";
      return;
    }
    else
    {
      ReportError("String table file " + stringTableFile + " exists but load failed!");
      // AMJU_TRACKING
    }
  }
  else
  {
    std::cout << "Preferred language is " << language << " but no string table.\n";
  }

  // No exact match. We want to get the closest match we have.
  // Try chopping off anything after the 2-char country code
  // (TODO Is this a good strategy?)
  stringTableFile = language.substr(0, 2) + ".txt";
  if (MyFileExists(stringTableFile))
  {
    if (Localise::LoadStringTable(stringTableFile))
    {
      std::cout << "Loaded fallback string table file " << stringTableFile << "\n";
      return;
    }
    else
    {
      ReportError("String table file " + stringTableFile + " exists but load failed!");
      // AMJU_TRACKING
    }
  }
  else
  {
    std::cout << "Fallback string table is " << stringTableFile << " but doesn't exist.\n";
  }

  std::cout << "Failed to load any string table, defaulting to 'en'.\n";
  // AMJU_TRACKING

  // Default to en.txt if all else failed
  Localise::LoadStringTable("en.txt");
}
  
void StartUpAfterCreateWindow()
{
  SetUpResourceLoaders();

  SetUpSound();

  SetUpGlueFile();

  LoadStringTableForPreferredLanguage();

  SetUpGui();

#ifdef YES_FPS_COUNTER
  // Set FPS counter
  Font* font = (Font*)TheResourceManager::Instance()->GetRes("font2d/arial-font.font");
  TheGame::Instance()->SetFrameTimeFont(font);
#endif

  SetInitialState();

  // Load this file one time only (but we can reload for development)
  GetComposerList().Load("Gui/composers.txt");
}
}

