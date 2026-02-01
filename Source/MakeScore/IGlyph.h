// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>
#include "Consts.h"
#include "Utils.h"

// * IGlyph *
// Interface for glyph types
struct IGlyph
{
  virtual ~IGlyph() = default;

  // Output drawing primitives for rendering, plus meta data for
  //  animation and MIDI notes.
  virtual std::string ToString() const = 0;

  virtual std::string CommentString() const = 0;

  void SetScale(float s) { scaleX = scaleY = s; }

  void SetPos(float x_, float y_) { x = x_; y = y_; }

  // Append this to get scale in X and Y, if scale is not 1 in both.
  // If scale is 1 for both axes, we return empty string.
  // This is to reduce the amount of unnecessary text we output.
  std::string AddScaleStringIfRequired() const
  {
    if (scaleX > 0.999f && scaleX < 1.001f && scaleY > 0.999f && scaleY < 1.001f)
    {
      return "";
    }
    return  ", " + Str(scaleX) + ", " + Str(scaleY);
  }

  float x = 0;
  // Default height is on single middle stave line for rhythm
  float y = DEFAULT_HEIGHT;
  float scaleX = 1.0f;
  float scaleY = 1.0f;

  // Time pos of this glyph in the piece. Units are crotchet beats from
  //  the start of the piece (zero based). 
  float m_crotchetTime = 0;
};
