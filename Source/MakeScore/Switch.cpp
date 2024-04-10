// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//

#include "Switch.h"

int SetSwitch(int& bitfield, Switch s)
{
  return (bitfield |= static_cast<int>(s));
}

int ClearSwitch(int& bitfield, Switch s)
{
  return (bitfield &= ~(static_cast<int>(s)));
}

