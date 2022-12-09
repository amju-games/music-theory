// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
//
// Sub project to convert easily-authorable music content
//  into list of glyphs.
//

#include "Attachment.h"
#include "Utils.h"

std::string Attachment::ToString() const 
{
  if (m_parent[LEFT])
  {
    const_cast<float&>(x) += m_parent[LEFT]->x;
  }

  std::string res = m_glyphText + ", " + 
    Str(x) + ", " + Str(y) + ", " +
    Str(scaleX) + ", " + Str(scaleY);

  return res;
}

