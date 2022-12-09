// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <cassert>
#include <vector>
#include "Glyph.h"
#include "Tie.h"

void Tie::SetPos()
{
  assert(m_leftGlyph != nullptr);
  assert(m_rightGlyph != nullptr);

  m_leftX = m_leftGlyph->x;
  m_leftGlyph->SetTieLeft(this);

  m_rightX = m_rightGlyph->x;
  m_rightGlyph->SetTieRight(this);
}

std::string Tie::ToString() const
{
  // Control points: start and end points in x; y value, depending
  //  on whether the tie is 'n' or 'u' shape. 
  // Inner control points: centre, and one near each end to give
  //  desired shape.
  float y = 1.f; // for 'n' shape -- TODO
                 // If U shape
  y = 1.1f;
  float w = m_rightX - m_leftX;
  const float TIE_ASPECT_RATIO = 8.f;
  float h = w / TIE_ASPECT_RATIO;
  // if u shape
  h = -h;
  float xoff = X_OFFSET_RIGHT * .6f;

  const float CP = 0.18f; // control point, for shape
  std::vector<float> coords =
  {
    m_leftX + xoff,  y, // we need to duplicate the first for spline calc
    m_leftX + xoff,  y,
    Interp(m_leftX, m_rightX, CP) + xoff, y + (h * 0.8f), // give shape
    Interp(m_leftX, m_rightX, .5f) + xoff, y + h, // centre
    Interp(m_leftX, m_rightX, (1.f - CP)) + xoff, y + (h * 0.8f), // give shape
    m_rightX + xoff, y,
    m_rightX + xoff, y, // TODO do we need this last one?
  };

  std::string res = "curve, ";
  int n = coords.size();
  for (int i = 0; i < n; i += 2)
  {
    res += Str(coords[i]     * scaleX) + ",";
    res += Str(coords[i + 1] * scaleY) + (i < (n - 2) ? ", " : "");
  }
  return res;
}

