// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

namespace Amju
{
// * Hints *
// Count of how many hints the user has.
class Hints
{
public:
  static int Get();
  static void Inc();
  static void Dec();
};
}

