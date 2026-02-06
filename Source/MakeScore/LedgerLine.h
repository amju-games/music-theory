// * MakeScore *
// (c) Copyright 2026 Juliet Colman

#pragma once

// * Ledger line width *
//  0. suppress ledger line/none required
//  1. single note, duration < sb
//  2. single sb
//  For notes in a chord, these are also possible:
//  3. notes < sb, offset to avoid overlapping
//  4. sbs, offset to avoid overlapping
enum class LedgerLineWidth
{
  NONE,
  SINGLE_NOTE,
  SINGLE_SB,
  MULTI_NOTE,
  MULTI_SB 
};

