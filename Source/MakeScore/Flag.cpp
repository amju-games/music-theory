// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Flag.h"

std::string Flag::ToString() const
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
