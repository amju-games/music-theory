// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include "precomp.h" // first include

#include <chrono>
#include <ctime>
#include <Directory.h>
#include <Screenshot.h> 
#include "PFScreenshot.h"

namespace Amju
{
static std::string get_current_timestamp() 
{
    // 1. Get the current time point
    auto now = std::chrono::system_clock::now();

    // 2. Convert to time_t (seconds since epoch)
    std::time_t time_now = std::chrono::system_clock::to_time_t(now);

    // 3. Convert to local time structure safely
    std::tm tm_now;
    #if defined(_WIN32)
        localtime_s(&tm_now, &time_now); // Windows safe version
    #else
        localtime_r(&time_now, &tm_now); // POSIX/Linux safe version
    #endif

    // 4. Format into a buffer and return as std::string
    char buffer[30];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d-%H.%M.%S", &tm_now);

    return std::string(buffer);
}
  
static std::string GetScreenshotDir()
{
  const std::string DIR_NAME = "Piano Fest Screenshots/";
  const std::string dir = GetDesktopDir() + DIR_NAME;
  MkDir(dir);
  return dir;
}

std::string SavePFScreenshot(int scale)
{
  // By default, save in Desktop/Piano Fest Screenshots/
  const std::string dir = GetScreenshotDir();
  const std::string filename = get_current_timestamp() + "_scr.png";
  const std::string full = dir + filename;
  SaveScreenshot(full, scale); 
  return full;
}
}

