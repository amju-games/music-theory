// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
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

KeySig TransposeKeySig(KeySig ks, int tr)
{
  // TODO Choose to favour sharp or flat key sigs

  // These are the key sigs for each tonic in sequence, i.e. the key sig
  //  for C major, then for C#/Db major, then D major, etc.
  const KeySig KEYS[] = 
  {
    KEYSIG_0_SHARP, // C   Also KEYSIG_0_FLAT
    KEYSIG_5_FLAT,  // Db  Also KEYSIG_7_SHARP (C#)
    KEYSIG_2_SHARP, // D
    KEYSIG_3_FLAT,  // Eb
    KEYSIG_4_SHARP, // E
    KEYSIG_1_FLAT,  // F
    KEYSIG_6_FLAT,  // Gb  Also KEYSIG_6_SHARP (F#)
    KEYSIG_1_SHARP, // G
    KEYSIG_4_FLAT,  // Ab
    KEYSIG_3_SHARP, // A
    KEYSIG_2_FLAT,  // Bb
    KEYSIG_5_SHARP, // B   Also KEYSIG_7_FLAT (Cb)

    // Same series, but with alternative key sigs. Only those elements
    //  matter, so the rest are set to -1.
    KEYSIG_0_FLAT,
    KEYSIG_7_SHARP,
    (KeySig)-1, 
    (KeySig)-1,
    (KeySig)-1,
    (KeySig)-1,
    KEYSIG_6_SHARP,
    (KeySig)-1,
    (KeySig)-1,
    (KeySig)-1,
    (KeySig)-1,
    KEYSIG_7_FLAT
  };

  // To transpose a key sig, we find it in the above array, then add
  //  tr to the index to get the index of the new key, using 
  //  modulo-12 addition.
  int i = 0;
  while (KEYS[i] != ks && i < 24)
  {
    i++;
  }
  i %= 12;
  i += tr;
  while (i < 0)
  {
    i += 12;
  }
  i %= 12;
  KeySig newKs = KEYS[i];
  return newKs;
}

