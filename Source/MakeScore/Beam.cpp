// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Beam.h"

std::string Beam::ToString() const
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

  // Increase in y from left end to right end of beam, which depends
  //  on gradient.
  // Get gradient from the primary beam, if there is one set.
  if (parent)
  {
    const_cast<float&>(gradient) = parent->gradient;
  }
  float yinc = gradient * (xmax - xmin);

  return "quad, " +
    Str((xmax + xoff) * scaleX) + ", " + Str((ymin + yinc) * scaleY) + ", " +
    Str((xmax + xoff) * scaleX) + ", " + Str((ymax + yinc) * scaleY) + ", " +
    Str((xmin + xoff) * scaleX) + ", " + Str(ymax * scaleY) + ", " +
    Str((xmin + xoff) * scaleX) + ", " + Str(ymin * scaleY);
}
