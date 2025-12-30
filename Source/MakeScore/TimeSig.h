// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

// Rather than a general solution for any conceivable time signature,
//  we just handle basic time sigs we expect to encounter. This means
//  in theory we can get good test coverage?
enum class TimeSig
{
  TIME_SIG_NONE,
  TIME_SIG_TWO_FOUR,
  TIME_SIG_THREE_FOUR,
  TIME_SIG_FOUR_FOUR,
  TIME_SIG_FIVE_FOUR,
  TIME_SIG_COMMON,
  TIME_SIG_CUT_COMMON,
  TIME_SIG_THREE_EIGHT,
  TIME_SIG_SIX_EIGHT,
  TIME_SIG_NINE_EIGHT,
  TIME_SIG_TWELVE_EIGHT,
};

// For the given time sig, return the number of crotchet beats in a bar.
//  and the note/rest duration multiplier.
//  This helps with compound times, so e.g. in 6/8, there
//  are two crotchet beats, and note and rest durations should be multiplied
//  by 2/3.  
using Beats_CrotchetValue = std::pair<int, float>;
Beats_CrotchetValue GetNumBeatsAndCrotchetValue(TimeSig);

// Return true if time sig is a compound time sig.
bool IsCompoundTime(TimeSig);

// Convert string to time sig, assume we have already checked that the
//  string is a valid time sig so no error status is returned.
TimeSig GetTimeSig(std::string s);

// Convert time sig to output string.
std::string GetStr(TimeSig ts);

// Return true if string is a recognised time sig input.
bool IsTimeSig(std::string s);
