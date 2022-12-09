// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>

enum class TimeSig
{
  TWO_FOUR,
  THREE_FOUR,
  FOUR_FOUR,
  COMMON,
  CUT_COMMON
};

TimeSig GetTimeSig(std::string s);

std::string GetStr(TimeSig ts);

bool IsTimeSig(std::string s);
