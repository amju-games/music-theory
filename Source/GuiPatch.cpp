#include <LoadVec2.h>
#include <StringUtils.h>
#include "GuiPatch.h"

namespace Amju
{
const char* GuiPatch::NAME = "patch";

bool GuiPatch::Load(File* f)
{
  if (!GuiSprite::Load(f)) // base class load
    return false;

  // Load corner size: if only one float is given, we want square corners,
  //  taking screen aspect ratio into account.
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("GuiPatch: expected corner size vec2 or corner width.");
    return false;
  }
  auto strs = Split(s, ',');
  if (strs.size() == 1)
  {
    // Square corners
    m_cornerSize.x = ToFloat(s);
    m_cornerSize.y = m_cornerSize.x;
    m_squareCorners = true;
  }
  else if (!ToVec2(s, &m_cornerSize))
  {
    f->ReportError("GuiPatch: expected corner size vec2.");
    return false;
  }

  return true;
}

void GuiPatch::Draw()
{
  AddToBatch(this);

  // Store current state of transform and colour, which we use later when
  //  we draw the batch of all tris using the current texture.
  // TODO At some point, we will need to store other stuff, e.g. current shader.
  m_combinedTransform.ModelView();
  m_combinedColour = AmjuGL::GetColour();
}

void GuiPatch::AddToTrilist(AmjuGL::Tris& triList) 
{
  if (!IsVisible())
    return;

  Vec2f size = GetSize();
  Vec2f pos = GetCombinedPos();

  float asp = 1.f;
  if (m_squareCorners)
  {
    // Get aspect ratio so corners are square, if flag set.
    int vx, vy, vw, vh;
    AmjuGL::GetViewport(&vx, &vy, &vw, &vh);
    asp = (float)vw / (float)vh;
  }

  // 4 x-coords, left to right
  float x[4] = { pos.x, pos.x + m_cornerSize.x, pos.x + size.x - m_cornerSize.x, pos.x + size.x };
  // 4 y-coords, top to bottom (+ve y is UP)
  float y[4] = { pos.y, pos.y - m_cornerSize.y * asp, pos.y - size.y + m_cornerSize.y * asp, pos.y - size.y };
  const float Z = 0;

  // Coords of the 16 points
  Vec3f v[16] =
  {
    Vec3f(x[0], y[0], Z) * m_combinedTransform,
    Vec3f(x[1], y[0], Z) * m_combinedTransform,
    Vec3f(x[2], y[0], Z) * m_combinedTransform,
    Vec3f(x[3], y[0], Z) * m_combinedTransform,
                         
    Vec3f(x[0], y[1], Z) * m_combinedTransform,
    Vec3f(x[1], y[1], Z) * m_combinedTransform,
    Vec3f(x[2], y[1], Z) * m_combinedTransform,
    Vec3f(x[3], y[1], Z) * m_combinedTransform,
                         
    Vec3f(x[0], y[2], Z) * m_combinedTransform,
    Vec3f(x[1], y[2], Z) * m_combinedTransform,
    Vec3f(x[2], y[2], Z) * m_combinedTransform,
    Vec3f(x[3], y[2], Z) * m_combinedTransform,
                         
    Vec3f(x[0], y[3], Z) * m_combinedTransform,
    Vec3f(x[1], y[3], Z) * m_combinedTransform,
    Vec3f(x[2], y[3], Z) * m_combinedTransform,
    Vec3f(x[3], y[3], Z) * m_combinedTransform
  };

  // uv coords of cell
  const float du = 1.f / static_cast<float>(m_numCellsXY.x);
  const float dv = 1.f / static_cast<float>(m_numCellsXY.y);
  const float u0 = du * static_cast<float>(m_cell % m_numCellsXY.x);
  const float v0 = 1.f - dv * static_cast<float>(m_cell / m_numCellsXY.x); // x, not y
  const float u1 = u0 + du;
  const float v1 = v0 - dv;
  // Mid points of uv cell
  const float uh = u0 + du * .5f;
  const float vh = v0 - dv * .5f;

  // Vertices: position and uv coord of each point
  AmjuGL::Vert verts[16] =
  {
    AmjuGL::Vert(v[0].x,  v[0].y,  v[0].z,    u0, v0,   0, 1, 0),
    AmjuGL::Vert(v[1].x,  v[1].y,  v[1].z,    uh, v0,   0, 1, 0),
    AmjuGL::Vert(v[2].x,  v[2].y,  v[2].z,    uh, v0,   0, 1, 0),
    AmjuGL::Vert(v[3].x,  v[3].y,  v[3].z,    u1, v0,   0, 1, 0),
    AmjuGL::Vert(v[4].x,  v[4].y,  v[4].z,    u0, vh,   0, 1, 0),
    AmjuGL::Vert(v[5].x,  v[5].y,  v[5].z,    uh, vh,   0, 1, 0),
    AmjuGL::Vert(v[6].x,  v[6].y,  v[6].z,    uh, vh,   0, 1, 0),
    AmjuGL::Vert(v[7].x,  v[7].y,  v[7].z,    u1, vh,   0, 1, 0),
    AmjuGL::Vert(v[8].x,  v[8].y,  v[8].z,    u0, vh,   0, 1, 0),
    AmjuGL::Vert(v[9].x,  v[9].y,  v[9].z,    uh, vh,   0, 1, 0),
    AmjuGL::Vert(v[10].x, v[10].y, v[10].z,   uh, vh,   0, 1, 0),
    AmjuGL::Vert(v[11].x, v[11].y, v[11].z,   u1, vh,   0, 1, 0),
    AmjuGL::Vert(v[12].x, v[12].y, v[12].z,   u0, v1,   0, 1, 0),
    AmjuGL::Vert(v[13].x, v[13].y, v[13].z,   uh, v1,   0, 1, 0),
    AmjuGL::Vert(v[14].x, v[14].y, v[14].z,   uh, v1,   0, 1, 0),
    AmjuGL::Vert(v[15].x, v[15].y, v[15].z,   u1, v1,   0, 1, 0)
  };

  // Add corner tris/rects
  AmjuGL::Tris tris;
  tris.push_back(AmjuGL::Tri(verts[5], verts[1], verts[0]));
  tris.push_back(AmjuGL::Tri(verts[5], verts[0], verts[4]));

  tris.push_back(AmjuGL::Tri(verts[7], verts[3], verts[2]));
  tris.push_back(AmjuGL::Tri(verts[7], verts[2], verts[6]));

  tris.push_back(AmjuGL::Tri(verts[13], verts[9], verts[8]));
  tris.push_back(AmjuGL::Tri(verts[13], verts[8], verts[12]));

  tris.push_back(AmjuGL::Tri(verts[15], verts[11], verts[10]));
  tris.push_back(AmjuGL::Tri(verts[15], verts[10], verts[14]));

  // Add remaining tris/rects - the '+' in the middle
  tris.push_back(AmjuGL::Tri(verts[6], verts[2], verts[1]));
  tris.push_back(AmjuGL::Tri(verts[6], verts[1], verts[5]));

  tris.push_back(AmjuGL::Tri(verts[9], verts[5], verts[4]));
  tris.push_back(AmjuGL::Tri(verts[9], verts[4], verts[8]));

  tris.push_back(AmjuGL::Tri(verts[10], verts[6], verts[5]));
  tris.push_back(AmjuGL::Tri(verts[10], verts[5], verts[9]));

  tris.push_back(AmjuGL::Tri(verts[11], verts[7], verts[6]));
  tris.push_back(AmjuGL::Tri(verts[11], verts[6], verts[10]));

  tris.push_back(AmjuGL::Tri(verts[14], verts[10], verts[9]));
  tris.push_back(AmjuGL::Tri(verts[14], verts[9], verts[13]));

  for (auto& t : tris)
  {
    t.SetColour(m_combinedColour);
  }

  triList.insert(triList.end(), tris.begin(), tris.end());
}
}

