// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Beam.h"

std::string Beam::CommentString() const
{
  return "// Beam, more info coming" + LineEnd();
}

std::string Beam::ToString() const
{
  return "quad, "; // +
    //Str((xmax + xoff) * scaleX) + ", " + Str((ymin + yinc) * scaleY) + ", " +
    //Str((xmax + xoff) * scaleX) + ", " + Str((ymax + yinc) * scaleY) + ", " +
    //Str((xmin + xoff) * scaleX) + ", " + Str(ymax * scaleY) + ", " +
    //Str((xmin + xoff) * scaleX) + ", " + Str(ymin * scaleY);
}
