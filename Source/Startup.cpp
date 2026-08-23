// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#ifdef WIN32
// Hola! There's a batch file to copy DLLs to Build dirs: Scripts/Msvc/DLLs/copy_dlls.bat

#if defined(_DEBUG)
#pragma comment(lib, "../../../../../amjulib/Build/Debug/AmjuLibMsvc.lib")
#else
#pragma comment(lib, "../../../../../amjulib/Build/Release/AmjuLibMsvc.lib")
#endif 
#endif // WIN32

#include <iostream>
#include <AmjuGLWindowInfo.h>
#include <AmjuRand.h>
#include <BassSoundPlayer.h>
#include <CommandLineArgs.h>
#include <ConfigFile.h>
#include <CursorManager.h>
#include <Directory.h>
#include <FileImplGlue.h>
#include <Game.h>
#include <GlueFileMem.h>
#include <GuiButton.h> // set click wav
#include <iOSUtils.h>
#include <Localise.h>
#include <ObjMesh.h> // set bin or text resource loader
#include <Shader.h> // set custom loader, TODO temporarily, it breaks reloading
#include <SoundManager.h>
#include "BassPlayMidi.h"
#include "Consts.h"
#include "InitialState.h"
#include "Palette.h" // add resource
#include "SetUpFactories.h"

#ifdef AMJU_IOS
// just on device, where we create Version.h in release script
#include "Version.h"
#endif

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
#endif  // _DEBUG

#ifdef NDEBUG
#define YES_GLUE_FILE
#define YES_BINARY_OBJ_FILES
#define GLUE_FILE "data-win.glue"
#define MUSIC_GLUE_FILE "music-win.glue"
#endif // NDEBUG
#endif  // WIN32

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

void Stop()
{
  // Stop prog execution in Release build.
  exit(1);
}

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
#ifdef WIN32
  // In MSVC we set the working directory in project properties,
  //  so just rely on that to set the root dir to Assets/.
  // But if we are using glue files, we want to look in Build/CompiledAssets.
#ifdef YES_GLUE_FILE
  File::SetRoot("..\\Build\\CompiledAssets", "/");
  std::cout << "Setting File::Root to " << File::GetRoot() << " for glue files.\n";
#endif
#endif

#ifdef AMJU_IOS
  std::string dir = GetDataDir();

#ifndef YES_GLUE_FILE // How is this optional?
  dir += "/Assets/";
#endif // YES_GLUE_FILE

  File::SetRoot(dir, "/");
#endif // iOS

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
    Stop();
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
    Stop();
  }
#endif // YES_GLUE_FILE
}

// Load the Game Config File, which is writable, so used to persist 
//  game-wide info.
void LoadWritableConfig()
{
  const std::string FIRST_TIME_VERSION = "first-time-version";

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
      isFirstTime = false; // we have run before!
      std::cout << "First version was: \"" << gcf->GetValue(FIRST_TIME_VERSION, "**NOT SET**") << "\"\n";
    }
  }

#ifdef AMJU_IOS
  if (isFirstTime)
  {
    gcf->Set(FIRST_TIME_VERSION, VERSION_STRING);
    gcf->Save();
    std::cout << "First time run! Setting first time version in game config.\n";
  }
#endif
}

void StartUpBeforeCreateWindow()
{
  std::cout << "*** AMJU PIANO FEST *** -- written by Juliet Colman 2026\n";

#if defined(WIN32) && defined(_DEBUG)
  // Set up MSVC mem leak reporting
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  Amju::Randomise();
  
  SetUpRootDir();
  
  LoadWritableConfig();
}

Resource* PianoShaderLoader(const std::string& resName)
{
  // Unfortunately we need this to specify the directory.
  // Strip off ".shader" from the name which is added to identify the resource type.
  std::string fullName = "Shaders/" + 
    AmjuGL::GetShaderDir() + "/" + 
    GetFileNoExt(resName);
  auto shader = AmjuGL::LoadShader(fullName); 
  return shader; 
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

  // Overwrite default shader resource loader so we can specify the path.
  // This doesn't work with reloading resources.
  // Resources should load through their loader function I guess.
  TheResourceManager::Instance()->AddLoader("shader", PianoShaderLoader);

  // Add palette loader: palettes are *.png.pal
  TheResourceManager::Instance()->AddLoader("pal", PaletteLoader);
}

void SetUpSound()
{
#ifdef AMJU_USE_BASS
  // Set sound player
  SoundManager* sm = TheSoundManager::Instance();
  BassSoundPlayer* bsp = new BassSoundPlayer;
  sm->SetImpl(bsp);

  // This is the player piano sound, running as a separate channel, independently
  //  of the currently playing song. 
  // TODO:
  // All soundfont names should be runtime configurable (by me, not player) 
std::cout << "Setting up player MIDI stream...\n";
  if (SetUpPlayerStream())
  {
std::cout << "  ..player MIDI stream is ok?\n";

    // This is just a test really. We need to check if we are connected,
    //  and attempt to connect periodically if not connected.
    TheBassMidiInput::Instance()->Connect();
  }
  else
  {
    std::cout << "\nSETTING PLAYER MIDI STREAM FAILED!\n";
  }

#endif // AMJU_USE_BASS
}

static void SetUpMisc()
{
// Urgh, TODO remove the need for this.
#if defined(WIN32) || defined(MACOSX)
  // Set image for cursor (e.g. hand with pointing finger for Wii controller).
  // If we don't care, just set any texture we have.
  // 2nd param is 'hotspot' pixel position, which, again, we don't care about.
  TheCursorManager::Instance()->Load("Image/hand.png", {}); 
#endif

  GuiButton::SetClickFilename(WAV_BUTTON_CLICK);
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

  SetUpFactories();

  SetUpMisc();

  SetInitialState();
}
}

