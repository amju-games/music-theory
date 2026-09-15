#pragma once

// * Get Version *
// Wrap platform-specific version files

#include <string>

#ifdef WIN32
#include "Windows/WindowsVersion.h"
#endif

namespace Amju
{
// * GetVersionString2 *
// Get version string: major.minor
inline std::string GetVersionString2()
{
#ifdef WIN32
  return STRINGIZE(VERSION_MAJOR) "." STRINGIZE(VERSION_MINOR);
#endif
}

// * GetVersionString3 *
// Get version string: major.minor.revision(patch)
inline std::string GetVersionString3()
{
#ifdef WIN32
  return STRINGIZE(VERSION_MAJOR) "." 
    STRINGIZE(VERSION_MINOR) "." 
    STRINGIZE(VERSION_REVISION);
#endif
}

}
