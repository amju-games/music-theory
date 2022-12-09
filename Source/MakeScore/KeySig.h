// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>
#include "Clef.h"

// Needed?
enum  KeySig
{
  KEYSIG_0_SHARP,
  KEYSIG_1_SHARP,
  KEYSIG_2_SHARP,
  KEYSIG_3_SHARP,
  KEYSIG_4_SHARP,
  KEYSIG_5_SHARP,
  KEYSIG_6_SHARP,
  KEYSIG_7_SHARP,

  KEYSIG_0_FLAT,
  KEYSIG_1_FLAT,
  KEYSIG_2_FLAT,
  KEYSIG_3_FLAT,
  KEYSIG_4_FLAT,
  KEYSIG_5_FLAT,
  KEYSIG_6_FLAT,
  KEYSIG_7_FLAT,
};

// Return true if input token defines a key sig.
bool IsKeySig(const std::string& s);

// Return key sig type from input token.
KeySig GetKeySig(const std::string& s);

// Generate output string for the given key sig, for the given clef, on 
//  the stave numbered from 0 and going down the page. 
// So for a single stave, staveNumber is 
//  zero. For the lower stave of a double-stave score (e.g. piano music), 
//  staveNumber is 1, etc.
std::string GetKeySigOutputString(KeySig ks, Clef clef, 
  int staveNumber, float x, float y, float scale);

