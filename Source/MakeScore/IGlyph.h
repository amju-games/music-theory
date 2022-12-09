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

  void SetScale(float s) { scaleX = scaleY = s; }

  void SetPos(float x_, float y_) { x = x_; y = y_; }

  float x = 0;
  // Default height is on single middle stave line for rhythm
  float y = DEFAULT_HEIGHT;
  float scaleX = 1.0f;
  float scaleY = 1.0f;
};
