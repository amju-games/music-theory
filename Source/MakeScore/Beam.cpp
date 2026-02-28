// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Beam.h"
#include "Consts.h"

std::string Beam::CommentString() const
{
  return "// Beam, more info coming" + LineEnd();
}

std::string Beam::ToString() const
{
  // Half height of beam, so we centre vertically?
  const float h = 0.02f; // TODO

  // Add y-coord which is set by Bar, and offset which we add to all quads.
  const float q = QUAD_Y_OFFSET + GetPos().y;

  // 4 corners of quad, CCW.
  // +ve y is UP
  // Add stem width to right end to line up exactly with stems.
  return "quad, " +
    Str(m_left.x * scaleX) + ", " + Str((m_left.y + h + q) * scaleY) + ", " +
    Str(m_left.x * scaleX) + ", " + Str((m_left.y - h + q) * scaleY) + ", " +
    Str((m_right.x + STEM_W) * scaleX) + ", " + Str((m_right.y - h + q) * scaleY) + ", " +
    Str((m_right.x + STEM_W) * scaleX) + ", " + Str((m_right.y + h + q) * scaleY) + 
    LineEnd(); 
}
