// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//

#include "Performance.h"

int SetPerformance(int& bitfield, Performance s)
{
  return (bitfield |= static_cast<int>(s));
}

int ClearPerformance(int& bitfield, Performance s)
{
  return (bitfield &= ~(static_cast<int>(s)));
}

