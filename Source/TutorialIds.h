// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

// * Unique ID for each Tutorial dialog displayed in code *

namespace Amju
{
// These IDs should be sequential starting from 0.
// Then we can use them as indices into arrays, e.g. to look up the
//  string for this unique tutorial ID.
static const int TUTORIAL_HELLO = 0;
static const int TUTORIAL_SWIPE = 1;

// Look up localised string from one of the IDs above
std::string LookupTutorialString(int id);
}

