// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//

#pragma once

enum Switch
{
  SW_STACCATO = 0x01,
  SW_ACCENT   = 0x02,
  SW_PAUSE    = 0x04,
};

// Set bit in bitfield. Also return new value for bitfield.
int SetSwitch(int& bitfield, Switch s);

// Clear bit in bitfield. Also return new value for bitfield.
int ClearSwitch(int& bitfield, Switch s);

