// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <GuiElement.h>
#include <Texture.h>
#include <TriList.h>

namespace Amju
{
class GuiLineDrawing : public GuiElement
{
public:
  static void AddToFactory();

  virtual void Draw() override;
  virtual bool Load(File*) override;
  virtual void Update() override;

  void AddPoint(const Vec2f& p);

  float GetTime() const;
  void SetIsPaused(bool isPaused);
  void SetColour(const Colour& col);

  // Load sequence of points from a file. This lets us make up a reusable library of strokes. 
  bool LoadPoints(File*);

protected:
  void Reset();
  void BuildTriList();
  void MakeInBetweenPoints();

protected:
  // A line drawing is a sequence of points.
  // We fill in the gaps between control points with a Catmull-Rom spline.
  std::vector<Vec2f> m_points;
  std::vector<Vec2f> m_controlPoints;

  RCPtr<TriList> m_triList;
  Colour m_fgCol;

  int m_index = 0; // index into m_points

  float m_speed = 1.0f;
  float m_startTime = 0;
  float m_time = 0;
  float m_maxTime = 1.0f; 
  float m_startWidth = 0.03f;
  float m_endWidth = 0.01f;

  // Where we are currently: interpolating between last 2 points
  Vec2f m_penPoint; 

  // 9-patch? So we can have rounded corners etc
  // Or just a circle, which we draw at the ends of line seg sequences.
  PTexture m_texture;

  bool m_isPaused = false;
};
}


