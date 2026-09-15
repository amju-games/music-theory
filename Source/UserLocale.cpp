#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <winnls.h>
#endif

#include "UserLocale.h"

namespace Amju
{
#ifdef AMJU_IOS
// Impl is in iOSUtils.mm
#endif

#ifdef WIN32 
std::string GetDevicePreferredLanguage()
{
  wchar_t wideLocale[LOCALE_NAME_MAX_LENGTH];

  // 1. Get the wide string from Windows
  if (GetUserDefaultLocaleName(wideLocale, LOCALE_NAME_MAX_LENGTH) == 0) {
    return "en"; // Fallback if the API fails
  }

  // 2. Calculate how big the narrow string needs to be
  int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wideLocale, -1, NULL, 0, NULL, NULL);

  // 3. Create the std::string and convert into it
  std::string resultString(sizeNeeded - 1, 0); // sizeNeeded includes the null terminator
  WideCharToMultiByte(CP_UTF8, 0, wideLocale, -1, &resultString[0], sizeNeeded, NULL, NULL);

  return resultString;
}
#endif
}
