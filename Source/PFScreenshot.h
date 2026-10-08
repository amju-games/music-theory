// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include <string>

namespace Amju
{
// * Screenshot *
// Take a screenshot.
// Save to disk; return path to png file.
// Scale is minification factor; 1 is full size, 2 is half size, 
//  3 is 1/3 size, etc.
std::string SavePFScreenshot(int scale = 2);
}

