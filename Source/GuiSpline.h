// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiElement.h>
#include <Texture.h>
#include <TriList.h>

namespace Amju
{
// * GuiSpline *
// Curved line, passes through a vector of control points (Catmull-Rom spline).
// Can be 'animated' in the sense of showing a portion of the spline, varying from
//  0..1 (i.e. the parametric 't' value).
class GuiSpline : public GuiElement
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  // Create a GuiElement editor object which lets us edit this node.
  GuiEdit* CreateEditor() override;

  void Draw() override;
  bool Load(File*) override;
  bool Save(File*) override;

  void Animate(float animValue) override;

  using ControlPoints = std::vector<Vec2f>;
  const ControlPoints& GetControlPoints() const;
  ControlPoints& GetControlPoints();

  //void AddPoint(const Vec2f& p);

  void AddControlPoint(const Vec2f& p);

  void SetColour(const Colour& col);

  void SetTexture(PTexture tex);

  // Call after adding control points (with AddControlPoint)
  void CreateFromControlPoints();

  void SetWidths(float w1, float w2);

protected:
  void Reset();
  void BuildTriList();
  void MakeInBetweenPoints();
  bool LoadPoints(File*);
  bool SavePoints(File*);

protected:
  // A line drawing is a sequence of points.
  // We fill in the gaps between control points with a Catmull-Rom spline.
  std::vector<Vec2f> m_points;
  ControlPoints m_controlPoints;

  // Total length of all segments 
  float m_totalLength = 0;

  RCPtr<TriList> m_triList;
  Colour m_fgCol;

  int m_index = 0; // index into m_points

  float m_startWidth = 0.03f;
  float m_endWidth = 0.01f;

  // A circle, which we use to draw rounded ends of line seg sequences.
  PTexture m_texture;
};
}


