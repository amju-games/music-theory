#pragma once

#include <string>
#include <vector>
#include <cstdio>
#include <stdexcept>

namespace Amju 
{
// Home-grown "format" function using safe C-style snprintf.
// This uses printf-style formatting strings and fails at run time
//  if the args don't match up. 
// Does not support changing the order of args in the format string,
//  which we need for localisation.
// So not great. TODO dump this when we have std::format support
//  across all devices.
template<typename... Args>
std::string format(const std::string& fmt, Args... args) 
{
  // 1. Ask snprintf how much space it needs (passing nullptr and 0 size)
  // Note: We use c_str() because snprintf expects a C-style string
  int size_needed = std::snprintf(nullptr, 0, fmt.c_str(), args...);
    
  if (size_needed < 0) 
  {
    throw std::runtime_error("Formatting error in string: " + fmt);
  }

  // 2. Allocate a buffer large enough for the text plus the null-terminator
  std::vector<char> buffer(size_needed + 1);

  // 3. Perform the actual format write into our safe buffer
  std::snprintf(buffer.data(), buffer.size(), fmt.c_str(), args...);

  // 4. Return as a clean std::string (excluding the null terminator)
  return std::string(buffer.data(), size_needed);
}
}

