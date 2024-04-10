// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm>
#include <cassert>
#include <vector>
#include "Curve.h"
#include "Utils.h"

std::string Curve::ToString() const
{
  assert(m_parent[0] != nullptr);
  assert(m_parent[1] != nullptr);

  const_cast<float&>(m_leftX) = m_parent[0]->x;
  const_cast<float&>(m_rightX) = m_parent[1]->x;

  const float Y_OFFSET = 0.5f; // everything is offset by this, sheesh
  float leftY = m_parent[0]->y + Y_OFFSET;
  float rightY = m_parent[1]->y + Y_OFFSET;

  // Control points: start and end points in x; y value, depending
  //  on whether the tie is 'n' or 'u' shape. 
  // Inner control points: centre, and one near each end to give
  //  desired shape.
  
  float w = m_rightX - m_leftX;
  const float ASPECT_RATIO = 5.f;
  const float MAX_H = 0.5f;
  float h = std::min(w / ASPECT_RATIO, MAX_H);

  // If U shape
  if (m_overNotUnder)
  {
    leftY += 0.2f;
    rightY += 0.2f;
  }
  else
  {
    leftY -= 0.2f;
    rightY -= 0.2f;
    h = -h;
  }

  float xoff = X_OFFSET_RIGHT * .6f;

  const float CP = 0.2f; // control point, for shape
  float centreY = (leftY + rightY) * .5f + h;
  std::vector<float> coords =
  {
    m_leftX + xoff,  leftY, // we need to duplicate the first for spline calc
    m_leftX + xoff,  leftY,
    Interp(m_leftX, m_rightX, CP) + xoff, Interp(centreY, leftY, CP), // give shape
    Interp(m_leftX, m_rightX, .5f) + xoff, centreY, // centre
    Interp(m_leftX, m_rightX, (1.f - CP)) + xoff, Interp(centreY, rightY, CP), // give shape
    m_rightX + xoff, rightY,
    m_rightX + xoff, rightY, // TODO do we need this last one?
  };

  std::string res = "curve, ";
  int n = coords.size();
  for (int i = 0; i < n; i += 2)
  {
    res += Str(coords[i] * scaleX) + ",";
    res += Str(coords[i + 1] * scaleY) + (i < (n - 2) ? ", " : "");
  }
  return res;
}
