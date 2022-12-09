// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"
#include "Utils.h"

enum class BeamLevel
{
  BEAM_LEVEL_1 = 0,
  BEAM_LEVEL_2 = 1,
};

float GetHeight(BeamLevel bl);

struct Beam : public IGlyph
{
  Beam() = default;
  Beam(BeamLevel level_, int left_, int right_) :
    level(level_), left(left_), right(right_) {}

  static constexpr float BEAM_HEIGHT = 0.06f;
  static constexpr float BEAM_Y_MIN = 0.770f;

  std::string ToString() const override
  {
    float ymin = y;
    float xoff = 0;
    if (upNotDown)
    {
      // Gap between beams/flags is half beam height, so y offset is * 1.5
      ymin -= BEAM_HEIGHT * 1.5f * GetHeight(level);
      ymin += BEAM_Y_MIN;
      xoff = X_OFFSET_RIGHT; // move to right as stem is on right of note  
    }
    float ymax = ymin + BEAM_HEIGHT;

    return "quad, " +
      Str((xmax + xoff) * scale) + ", " + Str(ymin * scale) + ", " +
      Str((xmax + xoff) * scale) + ", " + Str(ymax * scale) + ", " +
      Str((xmin + xoff) * scale) + ", " + Str(ymax * scale) + ", " +
      Str((xmin + xoff) * scale) + ", " + Str(ymin * scale);
  }

  BeamLevel level = BeamLevel::BEAM_LEVEL_1;
  int left = 0;
  int right = 0;
  float xmin = 0;
  float xmax = 0;

  // note type, so we know where to offset the quad corners
  bool upNotDown = true;
};

struct Flag : public Beam
{
  Flag(BeamLevel level_, int left_, int right_, bool stemDir_) :
    Beam(level_, left_, right_),
    stemLeftNotRight(stemDir_)
  {
  }

  std::string ToString() const override
  {
    float ymin = y;
    float xoff = 0;
    if (upNotDown)
    {
      ymin -= BEAM_HEIGHT * 1.5f * GetHeight(level);
      ymin += BEAM_Y_MIN;
      xoff = X_OFFSET_RIGHT; // move to right as stem is on right of note  
    }
    float ymax = ymin + BEAM_HEIGHT;

    float xmax_ = xmax;
    float xmin_ = xmin;
    const float FLAG_W = 0.2f; // flag is this proportion of full beam
    if (stemLeftNotRight)
    {
      xmax_ = xmin + (xmax - xmin) * FLAG_W;
    }
    else
    {
      xmin_ = xmax - (xmax - xmin) * FLAG_W;
    }

    return "quad, " +
      Str((xmax_ + xoff) * scale) + ", " + Str(ymin * scale) + ", " +
      Str((xmax_ + xoff) * scale) + ", " + Str(ymax * scale) + ", " +
      Str((xmin_ + xoff) * scale) + ", " + Str(ymax * scale) + ", " +
      Str((xmin_ + xoff) * scale) + ", " + Str(ymin * scale);
  }

  // Stem to which the flag is attached is on the left (true) or
  //  right (false).
  bool stemLeftNotRight = true;
};
