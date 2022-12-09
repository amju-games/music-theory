// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "KeySig.h"
#include "Utils.h"

/*
enum class KeySig
{
  KEYSIG_1_SHARP,
  KEYSIG_2_SHARP,
  KEYSIG_3_SHARP,
  KEYSIG_4_SHARP,
  KEYSIG_5_SHARP,
  KEYSIG_6_SHARP,
  KEYSIG_7_SHARP,

  KEYSIG_1_FLAT,
  KEYSIG_2_FLAT,
  KEYSIG_3_FLAT,
  KEYSIG_4_FLAT,
  KEYSIG_5_FLAT,
  KEYSIG_6_FLAT,
  KEYSIG_7_FLAT,
};
*/

bool IsKeySig(const std::string& s)
{
  return Contains(s, "key");
}

// Return key sig type from input token.
KeySig GetKeySig(const std::string& s)
{
  // Format is key-s-<n> key-f-<n>
  if (s.size() < 7)
  {
    std::cout << "// *ERROR* Bad key sig: " << s << "\n";
    return KEYSIG_0_SHARP;
  }

  const char* INPUT_TOKEN_KEYSIG_SHARP = "key-s-";
  const char* INPUT_TOKEN_KEYSIG_FLAT  = "key-f-";

  if (s.substr(0, 6) == INPUT_TOKEN_KEYSIG_SHARP)
  {
    int n = atoi(s.substr(6).c_str());
    return KeySig(KEYSIG_0_SHARP + n);
  }
  else if (s.substr(0, 6) == INPUT_TOKEN_KEYSIG_FLAT)
  {
    int n = atoi(s.substr(6).c_str());
    return KeySig(KEYSIG_0_FLAT + n);
  }
  
  std::cout << "// *ERROR* Bad key sig: " << s << "\n";
  return KEYSIG_0_SHARP;
}

std::string GetKeySigOutputString(KeySig ks, Clef clef, 
  int staveNumber, float x, float y, float scale)
{
  if (ks == KEYSIG_0_SHARP || ks == KEYSIG_0_FLAT)
  {
    return "";
  }

  std::string res = "keysig-";

  if (ks >= KEYSIG_0_FLAT)
  {
    // Flat
    res += Str(ks - KEYSIG_0_FLAT) + "flat-";
  }
  else
  {
    // Sharp
    res += Str(ks - KEYSIG_0_SHARP) + "sharp-";
  }

  const std::string CLEF_STR[] = { "treble", "bass", "alto", "tenor" };
  res += CLEF_STR[static_cast<int>(clef)];

  res += ", " + Str(x) + ", " + Str(y) + 
         ", " + Str(scale) + ", " + Str(scale);
  return res;
}

