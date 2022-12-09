#include <AmjuFirst.h>
#include <AmjuGL.h>
#include <StringUtils.h>
#include <Directory.h>
#include <GlueFile.h>
#include <FileImplGlue.h>
#include <Game.h>
#include <AmjuTime.h>
#include <Pause.h>
#include <CursorManager.h>
#include "Consts.h"
#include "GSCopyAssets.h"
#include "GSTitle.h"
#include <AmjuFinal.h>

#if defined(WIN32) && defined(_DEBUG)
//#define WIN32_TEST_COPY_ASSETS
#endif

//#define FILECOPY_DEBUG

namespace Amju
{
bool CopyFromGlueFile(const std::string& destDir)
{
  // The glue file has already been loaded into mem
  GlueFile* gf = FileImplGlue::GetGlueFile();
  if (!gf)
  {
    return true;
  }
  Assert(gf);
  const std::string& srcGlueFilePath = File::GetRoot() + gf->GetFilename();

#ifdef FILECOPY_DEBUG
std::cout << "Glue file + path: '" << srcGlueFilePath << "'\n";
#endif

  Time glueTime = GetFileModifiedTime(srcGlueFilePath);

  Strings strs;
  gf->Dir(&strs);
  int size = strs.size();
  for (int i = 0; i < size; i++)
  {
    const std::string& subfile = strs[i];
#ifdef FILECOPY_DEBUG_2
    std::cout << "Found file " << subfile << " from glue file...\n";
#endif

    bool doCopy = (subfile == "roconfig.txt");

#if defined(WIN32) || defined(MACOSX)
    doCopy |= (subfile.size() > 5 && subfile.substr(0, 6) == "levels");
#endif

    if (doCopy)
    {
#ifdef FILECOPY_DEBUG_2
std::cout << "We DO want to copy this file!\n";
#endif
    }
    else
    {
      continue;
    }

    std::string outFileName = destDir + subfile;
    // Check if we should copy file - we should if it does not exist, or
    //  timestamp of glue file is more recent than the file.

    if (FileExists(outFileName))
    {
#ifdef FILECOPY_DEBUG_2
std::cout << "File already exists: " << outFileName << "\n";
#endif
      Time destTime = GetFileModifiedTime(outFileName);

      if (!(destTime < glueTime))
      {
#ifdef FILECOPY_DEBUG_2
std::cout << "  Don't copy, OS file is more recent than glue file. (Glue file time: "
          << glueTime.ToString() << ")\n";
#endif
        doCopy = false; // more recent copy already there
      }
    }

    if (doCopy)
    {
      unsigned int seekbase = 0;
      if (!gf->GetSeekBase(subfile, &seekbase))
      {
        std::cout << "Failed to find subfile " << subfile << " in glue file, but was listed in Dir!!\n";
        Assert(0);
        return false;
      }
      uint32 size = gf->GetSize(subfile);
      unsigned char* buf = new unsigned char[size + 1];
      gf->GetBinary(seekbase, size, buf);

      File outFile(false, File::STD); // no version info
      std::string outFileDir = GetFilePath(outFileName);
      if (!MkDir(outFileDir))
      {
        std::cout << "MkDir failed: " << outFileDir << "\n";
        Assert(0);
        return false;
      }
#ifdef FILECOPY_DEBUG
std::cout << "Attempting to open file for writing: '" << outFileName << "'\n";
#endif

      if (!outFile.OpenWrite(outFileName, 
        0, // version
        true, // is binary
        false, // use root
        true // truncate 
      ))
      {
#ifdef FILECOPY_DEBUG
        std::cout << "Failed to open file " << outFileName << " for writing.\n";
#endif
        Assert(0);
        return false;
      }
      if (!outFile.WriteBinary((char*)buf, size))
      {
#ifdef FILECOPY_DEBUG
        std::cout << "Failed to write binary data to " << outFileName << ", size: " << size << "\n";
#endif
        Assert(0);
        return false;
      }
      delete [] buf; // whoops
    } // if doCopy
  } // next file in glue file
  return true;
}

GSCopyAssets::GSCopyAssets()
{
}

void GSCopyAssets::Update()
{
  // Counter so we draw window before copying
  static int done = 0;
  done++;
  if (done == 2)
  {
#ifdef FILECOPY_DEBUG
    std::cout << "Copying files to Save Dir as required...\n";
#endif      
    static std::string saveDir = GetSaveDir(APPNAME); 

#ifdef FILECOPY_DEBUG
std::cout << "\nSave Dir: " << saveDir << "\n";
#endif

    CopyFromGlueFile(saveDir);

    // All copied - go to next state 
    TheGame::Instance()->SetCurrentState(TheGSTitle::Instance());
  }
}

void GSCopyAssets::Draw()
{
}

void GSCopyAssets::Draw2d()
{
}

void GSCopyAssets::OnActive()
{
  GameState::OnActive();

  // Before loading anything, copy the files necessary to show logo or 'please wait' indicator.
  AmjuGL::SetClearColour(Colour(0, 0, 0, 1));
}

} // namespace

