// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <cassert>
#include <vector>
#include "Glyph.h"
#include "NoteAndChordBase.h"
#include "Tie.h"

void Tie::CalcPos()
{
  assert(m_leftGlyph != nullptr);
  assert(m_rightGlyph != nullptr);

  m_leftX = m_leftGlyph->x;
  m_leftGlyph->SetTieLeft(this);

  m_rightX = m_rightGlyph->x;
  m_rightGlyph->SetTieRight(this);
}

std::string Tie::CommentString() const
{
  return "// Tie" + LineEnd();
}

std::string Tie::ToString() const
{
  // Control points: start and end points in x; y value, depending
  //  on whether the tie is 'n' or 'u' shape. 
  // Inner control points: centre, and one near each end to give
  //  desired shape.

  bool stemUp = (m_leftGlyph->GetStaveLine() < 5);
  // We can assume the glyphs are notes/chords?
  if (auto n = dynamic_cast<NoteAndChordBase*>(m_leftGlyph))
  {
    stemUp = n->GetStem().GetDirection() == StemDir::UP;
  }

  bool curveIsU = stemUp;
  float tieY = m_leftGlyph->y;  // same as right side -- RIGHT?!
  // Don't know why, but we are off by 0.5 here.
  // Yes I do, it's QUAD_Y_OFFSET -- this is a curve, not a quad, but
  //  I'm guessing it's the same deal.
  tieY += QUAD_Y_OFFSET;

  // Add or subtract to get above/below note head.
  const float Y_ADJUST = 0.06f;
  tieY += Y_ADJUST * (curveIsU ? -1.f : 1.f);

  float w = m_rightX - m_leftX;
  const float TIE_ASPECT_RATIO = 8.f;
  const float MAX_H = 0.2f; // In final print coords, so this is 2 stave gaps
  float h = std::min(w / TIE_ASPECT_RATIO, MAX_H);
  if (curveIsU)
  {
    h = -h;
  }

  // Offset the whole thing to the right
  float xoff = X_OFFSET_RIGHT * .6f;

  // Squish the end points together a bit.
  float xSquish = NOTE_HEAD_WIDTH * 0.7f;

  const float CP = 0.22f; // control point, for shape
  std::vector<float> coords =
  {
    // Left end point
    m_leftX + xoff + xSquish,  tieY,
    // Control point: gives desired shape to curve
    Interp(m_leftX + xSquish, m_rightX - xSquish, CP) + xoff, tieY + (h * 0.8f), 
    // Centre point of curve
    Interp(m_leftX, m_rightX, .5f) + xoff, tieY + h, 
    // Symmetrical control point to give shape
    Interp(m_leftX + xSquish, m_rightX - xSquish, (1.f - CP)) + xoff, tieY + (h * 0.8f), 
    // Right end point
    m_rightX + xoff - xSquish, tieY
  };

  std::string res = "curve, ";
  auto n = coords.size();
  for (int i = 0; i < n; i += 2)
  {
    res += Str(coords[i]     * scaleX) + ",";
    res += Str(coords[i + 1] * scaleY) + (i < (n - 2) ? ", " : "");
  }
  return res;
}

