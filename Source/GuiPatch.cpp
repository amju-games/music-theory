#include <LoadVec2.h>
#include "GuiPatch.h"

namespace Amju
{
const char* GuiPatch::NAME = "patch";

bool GuiPatch::Load(File* f)
{
  if (!GuiSprite::Load(f)) // base class load
    return false;

  // Load corner size
  if (!LoadVec2(f, &m_cornerSize))
  {
    f->ReportError("GuiPatch: expected corner size vec2.");
    return false;
  }

  return true;
}

void GuiPatch::AddToTrilist(AmjuGL::Tris& triList) 
{
  if (!IsVisible())
    return;

  Vec2f size = GetSize();
  Vec2f pos = GetCombinedPos();

//  AmjuGL::Tri tri;

  // Set the vertex colours to the colour which was current when Draw was called.
  // TODO Do this for each tri we add to tris
  //tri.SetColour(m_combinedColour.m_r, m_combinedColour.m_g, m_combinedColour.m_b, m_combinedColour.m_a);

  // Get aspect ratio so corners are square -- don't think we want this
  //int vx, vy, vw, vh; 
  //AmjuGL::GetViewport(&vx, &vy, &vw, &vh);
  //float asp = (float)vw / (float)vh;
  float asp = 1.f;

  // 4 x-coords, left to right
  float x[4] = { pos.x, pos.x + m_cornerSize.x, pos.x + size.x - m_cornerSize.x, pos.x + size.x };
    //float y[4] = { pos.y, pos.y - m_cornerRadius * asp, pos.y - size.y + m_cornerRadius * asp, pos.y - size.y };

  // 4 y-coords, top to bottom (+ve y is UP)
  float y[4] = { pos.y, pos.y - m_cornerSize.y * asp, pos.y - size.y + m_cornerSize.y * asp, pos.y - size.y };

  // Vertices: 4x4 grid 
  AmjuGL::Vert v[16] =
  {
    AmjuGL::Vert(x[0], y[0], 0,   0.0, 0,   0, 1, 0),
    AmjuGL::Vert(x[1], y[0], 0,   0.5, 0,   0, 1, 0),
    AmjuGL::Vert(x[2], y[0], 0,   0.5, 0,   0, 1, 0),
    AmjuGL::Vert(x[3], y[0], 0,   1.0, 0,   0, 1, 0),

    AmjuGL::Vert(x[0], y[1], 0,   0.0, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[1], y[1], 0,   0.5, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[2], y[1], 0,   0.5, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[3], y[1], 0,   1.0, 0.5,   0, 1, 0),

    AmjuGL::Vert(x[0], y[2], 0,   0.0, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[1], y[2], 0,   0.5, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[2], y[2], 0,   0.5, 0.5,   0, 1, 0),
    AmjuGL::Vert(x[3], y[2], 0,   1.0, 0.5,   0, 1, 0),

    AmjuGL::Vert(x[0], y[3], 0,   0.0, 1.0,   0, 1, 0),
    AmjuGL::Vert(x[1], y[3], 0,   0.5, 1.0,   0, 1, 0),
    AmjuGL::Vert(x[2], y[3], 0,   0.5, 1.0,   0, 1, 0),
    AmjuGL::Vert(x[3], y[3], 0,   1.0, 1.0,   0, 1, 0),
  };

  // Add corner tris/rects
  AmjuGL::Tris tris;
  tris.push_back(AmjuGL::Tri(v[5], v[1], v[0]));
  tris.push_back(AmjuGL::Tri(v[5], v[0], v[4]));

  tris.push_back(AmjuGL::Tri(v[7], v[3], v[2]));
  tris.push_back(AmjuGL::Tri(v[7], v[2], v[6]));

  tris.push_back(AmjuGL::Tri(v[13], v[9], v[8]));
  tris.push_back(AmjuGL::Tri(v[13], v[8], v[12]));

  tris.push_back(AmjuGL::Tri(v[15], v[11], v[10]));
  tris.push_back(AmjuGL::Tri(v[15], v[10], v[14]));

  // Add remaining tris/rects - the '+' in the middle
  tris.push_back(AmjuGL::Tri(v[6], v[2], v[1]));
  tris.push_back(AmjuGL::Tri(v[6], v[1], v[5]));

  tris.push_back(AmjuGL::Tri(v[9], v[5], v[4]));
  tris.push_back(AmjuGL::Tri(v[9], v[4], v[8]));

  tris.push_back(AmjuGL::Tri(v[10], v[6], v[5]));
  tris.push_back(AmjuGL::Tri(v[10], v[5], v[9]));

  tris.push_back(AmjuGL::Tri(v[11], v[7], v[6]));
  tris.push_back(AmjuGL::Tri(v[11], v[6], v[10]));

  tris.push_back(AmjuGL::Tri(v[14], v[10], v[9]));
  tris.push_back(AmjuGL::Tri(v[14], v[9], v[13]));

  for (auto& t : tris)
  {
    t.SetColour(m_combinedColour);
  }

  triList.insert(triList.end(), tris.begin(), tris.end());
}
}

