// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm>
#include <cassert>
#include "Hairpin.h"
#include "Utils.h"

std::string Hairpin::ToString() const
{
  assert(m_parent[0] != nullptr);
  assert(m_parent[1] != nullptr);

  const float X_OFFSET = 0.1f; // Not sure what this is for
  const_cast<float&>(m_leftX) = m_parent[0]->x + X_OFFSET;
  const_cast<float&>(m_rightX) = m_parent[1]->x + X_OFFSET;

  // +ve y is UP
  // Line thickness
  const float TH = 0.018f;
  // Height: half distance between lines at open end
  const float H = 0.07f;
  // Mid point, i.e. y-coord of point at closed end
  const float Y = y + QUAD_Y_OFFSET + H * .5f; 
  // Plus half height to centre vertically with other performance directions

  // Y coords are Y +/- hl at left end, to Y +/- hr at right end.
  float hl = 0;
  float hr = H;

  if (!m_isCresc)
  {
    std::swap(hl, hr);
  }

  // Example quad:
  //  stave-line = quad, 1, 0.470,  1, 0.4825,  0, 0.4825,  0, 0.470 

  std::string res = 
    "quad, " + 
    Str(m_rightX * scaleX) + ", " + Str((Y + hr) * scaleY)       + ", " +
    Str(m_rightX * scaleX) + ", " + Str((Y + hr + TH) * scaleY)  + ", " +
    Str(m_leftX * scaleX)  + ", " + Str((Y + hl + TH) * scaleY)  + ", " +
    Str(m_leftX * scaleX)  + ", " + Str((Y + hl) * scaleY)       +
    LineEnd() + 
    "quad, " + 
    Str(m_rightX * scaleX) + ", " + Str((Y - hr) * scaleY) + ", " +
    Str(m_rightX * scaleX) + ", " + Str((Y - hr + TH) * scaleY) + ", " +
    Str(m_leftX * scaleX)  + ", " + Str((Y - hl + TH) * scaleY) + ", " +
    Str(m_leftX * scaleX)  + ", " + Str((Y - hl) * scaleY);
    
  return res;
}

std::string Hairpin::CommentString() const 
{
  if (!m_parent[LEFT] || !m_parent[RIGHT])
  {
    return "// Hairpin *** NOT PROPERLY ATTACHED!!!! ***" + LineEnd();
  }

  auto res = std::string("// Hairpin: ") + 
    (m_isCresc ? "cresc." : "decresc.") + 
    ", Left parent: "
    + LineEnd()
    + m_parent[LEFT]->CommentString() 
    + "// Right parent:" 
    + LineEnd() 
    + m_parent[RIGHT]->CommentString();
  
  return res;
}

