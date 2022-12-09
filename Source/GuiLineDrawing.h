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

protected:
  void Reset();
  void BuildTriList();

protected:
  // A line drawing is a sequence of points.
  std::vector<Vec2f> m_points;
  RCPtr<TriList> m_triList;
  Colour m_fgCol;

  int m_index = 0; // index into m_points

  float m_speed = 1.0f;
  float m_time = 0;
  float m_maxTime = 1.0f; 
  float m_startWidth = 0.03f;
  float m_endWidth = 0.01f;

  // Where we are currently: interpolating between last 2 points
  Vec2f m_penPoint; 

  // 9-patch? So we can have rounded corners etc
  // Or just a circle, which we draw at the ends of line seg sequences.
  PTexture m_texture;

};
}


