#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <winnls.h>
#endif

#ifdef MACOSX
#include <vector>
#include <CoreFoundation/CoreFoundation.h>
#include <StringUtils.h>
#endif

#include "UserLocale.h"

namespace Amju
{
#ifdef AMJU_IOS
// Impl is in iOSUtils.mm
#endif

#ifdef MACOSX
std::string GetDevicePreferredLanguage()
{
  std::string localeStr = "en"; // fallback

  // 1. Get the current system locale object
  CFLocaleRef currentLocale = CFLocaleCopyCurrent();
  if (!currentLocale) 
  {
std::cout << "No current locale found! Falling back to " 
  << localeStr << "\n";
    return localeStr;
  }

  // 2. Extract the BCP-47 identifier string
  CFStringRef localeIdentifier = CFLocaleGetIdentifier(currentLocale);
  if (localeIdentifier) 
  {
    // 3. Convert the Apple CFStringRef into a standard C++ std::string
    // Determine the required buffer length
    CFIndex length = CFStringGetLength(localeIdentifier);
    CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
        
    std::vector<char> buffer(maxSize);
    if (CFStringGetCString(localeIdentifier, buffer.data(), maxSize, kCFStringEncodingUTF8)) 
    {
      localeStr = std::string(buffer.data());
    }
  }

  // 4. Free the memory allocated by CFLocaleCopyCurrent
  CFRelease(currentLocale);

  // 5. Normalise underscores to dashes
  localeStr = Amju::Replace(localeStr, "_", "-");

  return localeStr;
}
#endif // MACOSX

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

