// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>
#include "Consts.h"

struct IGlyph
{
  virtual ~IGlyph() = default;
  virtual std::string ToString() const = 0;

  void SetScale(float s) { scale = s; }

  float x = 0;
  // Default height is on single middle stave line for rhythm
  float y = DEFAULT_HEIGHT;
  float scale = 1.0f;
};
