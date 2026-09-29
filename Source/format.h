#if 0
// Enable this once we have {fmt} library installed

#pragma once

// Wrapper around C++20 <format>, which is not supported on 
//  Apple clang 13.
//
// Uses the {fmt} library where <format> is unavailable.
//
// To install the {fmt} library on Mac:
//    brew install fmt
//
// TODO Promote this to amjulib.

/*
*** Client code example

#include <iostream>
#include <string>
#include "format.h" // Include this file

int main() 
{
    int points = 42;
    std::string user = "Alice";

    // 1. Standard layout (English style)
    std::string msg_en = Amju::format("{0} scored {1} points!", user, points);
    std::cout << msg_en << '\n'; // Outputs: Alice scored 42 points!

    // 2. Swapped layout (for other languages)
    // Translators can rearrange the numbers safely without touching the code.
    std::string msg_loc = Amju::format("Points scored: {1} (By: {0})", user, points);
    std::cout << msg_loc << '\n'; // Outputs: Points scored: 42 (By: Alice)
}

*/

#include <version> // Contains standard library feature-test macros

// Check if the standard library natively supports C++20 std::format
#if defined(__cpp_lib_format)
    #include <format>
    namespace Amju 
    {
        // Use the native C++20 implementation if available
        using std::format;
    }
#else
    // Fall back to the external {fmt} library for older compilers
    #include <fmt/core.h>
    namespace Amju
    {
        // Pull fmt::format into our custom namespace
        using fmt::format;
    }
#endif

#endif // 0, remove once {fmt} available

