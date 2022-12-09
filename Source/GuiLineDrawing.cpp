// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiFactory.h>
#include <StringUtils.h>
#include <Timer.h>
#include <Vec3.h>
#include "GuiLineDrawing.h"

namespace Amju
{
static GuiElement* CreateLineDrawing()
{
  return new GuiLineDrawing;
}

void GuiLineDrawing::AddToFactory()
{
  TheGuiFactory::Instance()->Add("line-drawing", CreateLineDrawing);
}

void GuiLineDrawing::Reset()
{
  m_triList = nullptr;
  m_index = 0;
  m_time = 0;
}


static Vec2f CatmullRomSpline(float t, Vec2f p1, Vec2f p2, Vec2f p3, Vec2f p4)
{
  float t2 = t*t;
  float t3 = t*t*t;
  Vec2f v; // Interpolated point

  /* Catmull Rom spline Calculation */
  v.x = ((-t3 + 2 * t2 - t)*(p1.x) + (3 * t3 - 5 * t2 + 2)*(p2.x) + (-3 * t3 + 4 * t2 + t)* (p3.x) + (t3 - t2)*(p4.x)) / 2;
  v.y = ((-t3 + 2 * t2 - t)*(p1.y) + (3 * t3 - 5 * t2 + 2)*(p2.y) + (-3 * t3 + 4 * t2 + t)* (p3.y) + (t3 - t2)*(p4.y)) / 2;

  return v;
}

void GuiLineDrawing::BuildTriList()
{
  AmjuGL::Tris tris;
  for (int i = 1; i < m_index; i++)
  {
    const Vec2f& p0 = m_points[i - 1];
    const Vec2f& p1 = m_points[i];
    Vec2f dir = p1 - p0;
    Vec3f dir3(dir.x, dir.y, 0);
    Vec3f perp3 = CrossProduct(dir3, Vec3f(0, 0, 1));
    perp3.Normalise();
    Vec2f perp(perp3.x, perp3.y);
    float d = static_cast<float>(i) / static_cast<float>(m_points.size());
    float w = m_startWidth + (m_endWidth - m_startWidth) * d; // width
    Vec2f p[4] = 
    {
      p0 + perp * w,
      p1 + perp * w,
      p1 - perp * w,
      p0 - perp * w
    };

    AmjuGL::Tri t[2];

    const float Z = 0.5f;
    float u = 0;
    float v = 0;
    AmjuGL::Vert verts[4] =
    {
      AmjuGL::Vert(p[0].x, p[0].y, Z, u, v, 0, 1.0f, 0),
      AmjuGL::Vert(p[1].x, p[1].y, Z, u, v, 0, 1.0f, 0),
      AmjuGL::Vert(p[2].x, p[2].y, Z, u, v, 0, 1.0f, 0),
      AmjuGL::Vert(p[3].x, p[3].y, Z, u, v, 0, 1.0f, 0)
    };

    t[0].m_verts[0] = verts[0];
    t[0].m_verts[1] = verts[1];
    t[0].m_verts[2] = verts[2];

    t[1].m_verts[0] = verts[0];
    t[1].m_verts[1] = verts[2];
    t[1].m_verts[2] = verts[3];

    tris.push_back(t[0]);
    tris.push_back(t[1]);
  }
  m_triList = Amju::MakeTriList(tris);
}

void GuiLineDrawing::Draw()
{
  Vec2f pos = GetCombinedPos();

  AmjuGL::PushMatrix();
  AmjuGL::SetColour(Colour(1, 0, 0, 1));
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Draw(m_triList);
  AmjuGL::PopMatrix();
}

bool GuiLineDrawing::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // Max time
  if (!f->GetFloat(&m_maxTime))
  {
    f->ReportError("Expected max time for line drawing.");
    return false;
  }

  // Width, which is interpolated over line
  if (!f->GetFloat(&m_startWidth))
  {
    f->ReportError("Expected start width for line drawing.");
    return false;
  }

  if (!f->GetFloat(&m_endWidth))
  {
    f->ReportError("Expected end width for line drawing.");
    return false;
  }

  // Load control points
  std::vector<Vec2f> points;
  std::string line;
  while (f->GetDataLine(&line))
  {
    // Split line
    Strings strs = Split(line, ',');
    int n = strs.size();
    if (n != 2)
    {
      break;
    }
    Vec2f pos(ToFloat(strs[0]), ToFloat(strs[1]));
    points.push_back(pos);
  }

  // Make in between points from control points
  int n = points.size() - 3;
  for (int i = 0; i < n; i++)
  {
    float t = 0;
    while (t < 1.0f)
    {
      Vec2f v = CatmullRomSpline(t, points[i], points[i + 1], points[i + 2], points[i + 3]);
      v.x *= m_size.x;
      v.y *= m_size.y;
      m_points.push_back(v);
      t += 0.04f; // TODO 
    }
  }

  return true;
}

void GuiLineDrawing::AddPoint(const Vec2f& p)
{
  m_points.push_back(p);
  m_index = m_points.size();
  BuildTriList();
}

void GuiLineDrawing::Update()
{
  float dt = TheTimer::Instance()->GetDt();

  m_time += dt;

  // Final index into m_points
  float d = m_time / m_maxTime; // distance 0..1
  int oldIndex = m_index;
  m_index = static_cast<int>(static_cast<float>(m_points.size()) * d);
  if (m_index > static_cast<int>(m_points.size()))
  {
    m_index = static_cast<int>(m_points.size());
    m_time = m_maxTime;
  }

  if (oldIndex != m_index)
  {
    BuildTriList();
  }
}

}

