// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Quad.h"
#include "Utils.h"

Quad::Quad(float xmin, float ymin, float xmax, float ymax)
{
  m_cornerXY[0] = xmin;
  m_cornerXY[1] = ymin;
  m_cornerXY[2] = xmax;
  m_cornerXY[3] = ymin;
  m_cornerXY[4] = xmax;
  m_cornerXY[5] = ymax;
  m_cornerXY[6] = xmin;
  m_cornerXY[7] = ymax;
}

Quad::Quad(float cornerXY[8])
{
  for (int i = 0; i < 8; i++)
  {
    m_cornerXY[i] = cornerXY[i];
  }
}

std::string Quad::ToString() const 
{
  return "quad, " + 
    Str(m_cornerXY[0]) + ", " + Str(m_cornerXY[1]) + ", " +
    Str(m_cornerXY[2]) + ", " + Str(m_cornerXY[3]) + ", " +
    Str(m_cornerXY[4]) + ", " + Str(m_cornerXY[5]) + ", " +
    Str(m_cornerXY[6]) + ", " + Str(m_cornerXY[7]);
}

