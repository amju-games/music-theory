// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class TimeSig
{
  TIME_SIG_NONE,
  TIME_SIG_TWO_FOUR,
  TIME_SIG_THREE_FOUR,
  TIME_SIG_FOUR_FOUR,
  TIME_SIG_COMMON,
  TIME_SIG_CUT_COMMON
};

TimeSig GetTimeSig(std::string s);

std::string GetStr(TimeSig ts);

bool IsTimeSig(std::string s);
