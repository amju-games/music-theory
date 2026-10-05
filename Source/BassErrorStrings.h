#pragma once

#include <cstdint> // uint32_t
#include <string>

namespace Amju
{
std::string GetBassErrorString(int bassErrorCode);

std::string GetBassVersionString(uint32_t version);

// Calls BASS_ErrorGetCode; returns that code and prints error
//  message if code is non-zero.
int CheckBassStatus();
}

