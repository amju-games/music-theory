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

float GuiLineDrawing::GetTime() const
{
  return m_time;
}

void GuiLineDrawing::SetIsPaused(bool isPaused)
{
  m_isPaused = isPaused;
}

void GuiLineDrawing::Reset()
{
  m_triList = nullptr;
  m_index = 0;
  m_time = -m_startTime;
  m_isPaused = false;
}

void GuiLineDrawing::SetColour(const Colour& col)
{
  m_fgCol = col;
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

  Vec2f p[4];

  for (int i = 1; i < m_index; i++)
  {
    const Vec2f& p0 = m_points[i - 1];
    const Vec2f& p1 = m_points[i];
    Vec2f dir = p1 - p0;
    Vec3f dir3(dir.x, dir.y, 0);
    dir3.Normalise();
    Vec3f perp3 = CrossProduct(dir3, Vec3f(0, 0, 1));
    perp3.Normalise();
    Vec2f perp(perp3.x, perp3.y);

    // Calc line width here: either linear interp between start and end...
#define LINE_WIDTH_LINEAR
#ifdef LINE_WIDTH_LINEAR
    float d = static_cast<float>(i) / static_cast<float>(m_points.size());
#else 
    // Or vary based on current direction, so a bit more like calligraphy?
    float d = fabs(DotProduct(dir3, Vec3f(1, 0, 0)));
#endif

    float w = m_startWidth + (m_endWidth - m_startWidth) * d; 

	if (i == 1)
	{
		p[0] = p0 + perp * w;
		p[1] = p1 + perp * w;
		p[2] = p1 - perp * w;
		p[3] = p0 - perp * w;
	}
	else
	{
		p[0] = p[1];
		p[3] = p[2];
		p[1] = p1 + perp * w;
		p[2] = p1 - perp * w;
	}

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

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D); // for now
  AmjuGL::PushMatrix();
  PushColour();
  AmjuGL::SetColour(m_fgCol);
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Draw(m_triList);
  PopColour();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

bool GuiLineDrawing::Load(File* f)
{
  // Get name, pos, size
  if (!GuiElement::Load(f))
  {
    return false;
  }

  // Start time
  if (!f->GetFloat(&m_startTime))
  {
    f->ReportError("Expected start time for line drawing.");
    return false;
  }
  m_time = -m_startTime;

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

  // Colour
  std::string colour;
  if (!f->GetDataLine(&colour))
  {
    f->ReportError("Expected line drawing colour.");
    return false;
  }
  m_fgCol = FromHexString(colour);

  // Get points filename
  std::string pointsFilename;
  if (!f->GetDataLine(&pointsFilename))
  {
    f->ReportError("Expected points file name.");
    return false;
  }

  std::string path = GetFilePath(f->GetName());
  pointsFilename = path + "/" + pointsFilename;

  File pointsFile;
  if (!pointsFile.OpenRead(pointsFilename))
  {
    return false;
  }

  return LoadPoints(&pointsFile);
}

bool GuiLineDrawing::LoadPoints(File* f)
{
  // Load control points
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
    m_controlPoints.push_back(pos);
  }

  MakeInBetweenPoints();

  return true;
}

bool GuiLineDrawing::SavePoints(File* f)
{
  // Save points, not control points
  for (const Vec2f& p : m_points)
  {
    if (!f->Write(ToString(p.x) + ", " + ToString(p.y)))
    {
      return false;
    }
  }
  return true;
}

void GuiLineDrawing::MakeInBetweenPoints()
{
  // Make in between points from control points
  m_points.clear();
  int n = m_controlPoints.size() - 3;
  for (int i = 0; i < n; i++)
  {
    float t = 0;
    while (t < 1.0f)
    {
      Vec2f v = CatmullRomSpline(t, m_controlPoints[i], m_controlPoints[i + 1], m_controlPoints[i + 2], m_controlPoints[i + 3]);
      v.x *= m_size.x;
      v.y *= m_size.y;
      m_points.push_back(v);

t += 0.4f;
//      t += 0.04f; // TODO 
    }
  }
}

void GuiLineDrawing::AddPoint(const Vec2f& p)
{
//  m_controlPoints.push_back(p);
//  MakeInBetweenPoints();

  m_points.push_back(p);
  m_index = m_points.size();
  BuildTriList();

#ifdef _DEBUG
  std::cout << "Num control points: " << m_controlPoints.size() << " num points: " << m_points.size() << "\n";
#endif
}

void GuiLineDrawing::Update()
{
  if (m_isPaused)
  {
    return;
  }

  if (!IsVisible())
  {
    return;
  }

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

