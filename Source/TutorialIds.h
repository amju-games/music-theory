// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

// * Unique ID for each Tutorial dialog displayed in code *

namespace Amju
{
// These IDs should be sequential starting from 0.
// Then we can use them as indices into arrays, e.g. to look up the
//  string for this unique tutorial ID.
static const int TUTORIAL_HELLO = 0;
static const int TUTORIAL_SWIPE = 1;
static const int TUTORIAL_KB_MIDDLE_C = 2;
static const int TUTORIAL_KB_SWIPE = 3;
static const int TUTORIAL_TAP_DOOR = 4;

// Look up localised string from one of the IDs above
std::string LookupTutorialString(int id);
}

