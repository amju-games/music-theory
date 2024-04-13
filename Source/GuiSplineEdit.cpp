#include <Line2.h>
#include "GuiSpline.h"
#include "GuiSplineEdit.h"

namespace Amju
{
const char* GuiSplineEdit::NAME = "gui-spline-edit";

GuiSplineEdit::GuiSplineEdit()
{
} 

void GuiSplineEdit::RecalcGrabberPositions()
{
  m_grabbers.clear();

  const Vec2f pos = GetSpline()->GetCombinedPos();

  const auto& points = GetSpline()->GetControlPoints();
  for (const auto& p : points)
  {
    const Vec2f q = pos + p;
    m_grabbers.push_back(Rect(q.x - GRAB_SIZE_X, q.x + GRAB_SIZE_X, q.y - GRAB_SIZE_Y, q.y + GRAB_SIZE_Y));
  }
}

void GuiSplineEdit::HandleDragGrabber(const Vec2f& deltaPos)
{
  auto& points = GetSpline()->GetControlPoints();
  Assert(m_selectedGrabberIndex >= 0);
  Assert(m_selectedGrabberIndex < static_cast<int>(points.size()));
  Vec2f& p = points[m_selectedGrabberIndex];
  p += deltaPos;

  RecalcGrabberPositions();
  GetSpline()->CreateFromControlPoints();
  // TODO This should recalc the bounding rect too.
}

GuiSpline* GuiSplineEdit::GetSpline()
{
  GuiSpline* s = dynamic_cast<GuiSpline*>(GetChild());
  Assert(s);
  return s;
}

void GuiSplineEdit::Draw()
{
  GuiEdit::Draw();

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();
  AmjuGL::SetColour(Colour(0, 0, 1, 1));

  // Draw lines between control points, so we can see order
  const Vec2f pos = GetSpline()->GetCombinedPos();

  const auto& points = GetSpline()->GetControlPoints();
  for (int i = 1; i < static_cast<int>(points.size()); i++)
  {
    const Vec2f q0 = points[i - 1] + pos;
    const Vec2f q1 = points[i] + pos;
    AmjuGL::DrawLine(AmjuGL::Vec3(q0.x, q0.y, 0), AmjuGL::Vec3(q1.x, q1.y, 0));
  }

  PopColour();
  AmjuGL::Enable(AmjuGL::AMJU_TEXTURE_2D);
}

bool GuiSplineEdit::CreateNewControlPoint(const MouseButtonEvent& mbe)
{
  // See if we clicked on a line seg between two control points.
  const Vec2f p(mbe.x, mbe.y);
  const Vec2f pointsOffset = GetSpline()->GetCombinedPos();
  auto& points = GetSpline()->GetControlPoints();
  for (int i = 1; i < static_cast<int>(points.size()); i++)
  {
    const Vec2f q0 = points[i - 1] + pointsOffset;
    const Vec2f q1 = points[i] + pointsOffset;
    Line2 line(q0, q1);
    const float sqDist = line.SqDist(p);
    if (sqDist < 0.002f)
    {
      // Close enough! Add a new control point.
      points.insert(points.begin() + i, p - pointsOffset);
      RecalcGrabberPositions();
      return true;
    }
  }
  return false;
}

bool GuiSplineEdit::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_RIGHT)
  {
    // Right click on grabber: menu to delete it? Or keypress to delete highlighed grabber?

    // Right click NOT on a grabber: add a new control point? Hmm, we need to know where it goes in sequence.
    //  So you have to click on a line between 2 existing control points.

    // If you R click down on (or close enough to) a line seg joining two control points,
    //  add a new control point where you clicked.
    if (mbe.isDown &&
      CreateNewControlPoint(mbe))
    {
      return true;
    }

    return false;
  }
  else
  {

    return GuiEdit::OnMouseButtonEvent(mbe);
  }
}

}