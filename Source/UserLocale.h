#pragma once

#include <string>

namespace Amju
{
// TODO Go with the function name for now.
// Get the BCP-47 code for the user's language setting.
// Examples: "en-GB", "en-CA", "zh-HK", etc. 
// TODO Promote to amjulib.
std::string GetDevicePreferredLanguage();
}
