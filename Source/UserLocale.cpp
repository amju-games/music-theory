#include <iostream>

#ifdef WIN32
#include <windows.h>
#include <winnls.h>
#include <vector>
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
  // 1. First call: Determine the required buffer size (in characters)
  // MUI_LANGUAGE_NAME returns tags like "fr-FR", "en-GB"
  ULONG numLanguages = 0;
  ULONG bufferSize = 0;
  if (!GetUserPreferredUILanguages(
    MUI_LANGUAGE_NAME, &numLanguages, nullptr, &bufferSize)) 
  {
    std::cerr << "Failed to get buffer size. Error: " << GetLastError() << "\n";
    return "en-GB"; // Fallback
  }

  // 2. Allocate the buffer based on the returned size
  std::vector<wchar_t> buffer(bufferSize);

  // 3. Second call: Populate the buffer with the actual language data
  if (!GetUserPreferredUILanguages(
    MUI_LANGUAGE_NAME, &numLanguages, buffer.data(), &bufferSize)) 
  {
    std::cerr << "Failed to retrieve languages. Error: " << GetLastError() << "\n";
    return "en-GB"; // Fallback
  }

  // 4. Parse the double-null-terminated string sequence
  // The format is: "fr-FR\0en-GB\0\0"
  std::vector<std::wstring> preferredLanguages;
  const wchar_t* currentPtr = buffer.data();

  while (*currentPtr != L'\0') 
  {
    std::wstring lang(currentPtr);
    preferredLanguages.push_back(lang);

    // Move pointer forward past the current string and its null terminator
    currentPtr += lang.length() + 1;
  }

  // 5. Output the results
  std::wcout << L"Found " << numLanguages << L" preferred UI language(s):\n";
  for (size_t i = 0; i < preferredLanguages.size(); ++i) 
  {
    std::wcout << i + 1 << L": " << preferredLanguages[i] << L"\n";
  }

  if (preferredLanguages.empty())
  {
    return "en-GB"; // Fallback
  }

  // TODO We should change our algo to take multiple preferred languages
  //  into account.
  // For now, return the top language.

  // Convert top preferred wstring to string:
  // Get locale, needed to convert wstring to multibyte.
  wchar_t wideLocale[LOCALE_NAME_MAX_LENGTH];
  if (GetUserDefaultLocaleName(wideLocale, LOCALE_NAME_MAX_LENGTH) == 0)
  {
    return "en-GB"; // Fallback
  }
  auto sizeNeeded = preferredLanguages.front().length();
  std::string resultString(sizeNeeded - 1, 0); // sizeNeeded includes the null terminator
  WideCharToMultiByte(CP_UTF8, 0, wideLocale, -1, &resultString[0], static_cast<int>(sizeNeeded), NULL, NULL);

  return resultString;
}
#endif
}

