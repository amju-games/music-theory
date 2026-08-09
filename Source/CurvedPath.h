#pragma once

#include <cmath>
#include <iostream>
#include <numbers>
#include <vector>
#include <Vec2.h>

#ifndef M_PI
#define M_PI std::numbers::pi_v<float>
#endif

namespace Amju
{
// * Curved Path *
// Generates points along a curved path, with loops, figure-8s and a spiral at
//  one end.

struct SpiralConfig
{
  float durationT = 0.3f;     // What fraction of the path the spiral covers [0.0, 1.0]
  int turns = 2;              // Total full rotations
  float maxRadius = 50.0f;    // Peak radius of the spiral
  float startAngleRad = 0.0f; // Initial direction angle in radians
  bool clockwise = false;     // False = CCW, True = CW
  bool expandOutward = true;  // True = start center and expand; False = start wide and shrink
};

struct LoopConfig
{
  float startT;             // Where on the path [0, 1] it begins
  float endT;               // Where it ends
  float radius;             // Radius of the loop
  int rotations = 1;        // Number of full loops
  bool clockwise = false;   // Rotation direction
};

struct Figure8Config
{
  float startT;             // Where on the path [0, 1] it begins
  float endT;               // Where it ends
  float scale;              // Size of the figure-8
  int rotations = 1;        // Number of full figure-8s
};

struct PathConfig
{
  Vec2f startPos;
  Vec2f endPos;
  bool backwards = false; // if true, reverse through the path from the end.
 
  SpiralConfig spiral;
  std::vector<LoopConfig> loops;
  std::vector<Figure8Config> figureEights;
};

// * Curved Path Generator *
// Generates points on a curved path using a PathConfig.
class CurvedPathGenerator
{
private:
  PathConfig m_config;
  Vec2f m_dir;
  Vec2f m_tangent;
  Vec2f m_normal;

public:
  CurvedPathGenerator() = default;
 
  CurvedPathGenerator(const PathConfig& pc) { SetConfig(pc); }

  void SetConfig(const PathConfig& pc) 
  { 
    m_config = pc; 
    CalcDirTangentNormal();
  }

  const PathConfig& GetConfig() const { return m_config; }

  void CalcDirTangentNormal()
  {
    // Calculate base path direction (tangent) and perpendicular (normal)
    m_dir = m_config.endPos - m_config.startPos;
    float len = std::sqrt(m_dir.SqLen());
    
    // Fallback if start and end are exactly the same
    if (len < 0.0001f) 
    {
      m_dir = { 1.0f, 0.0f };
      len = 1.0f;
    }

    m_tangent = m_dir * (1.f/len);
    m_normal = { -m_tangent.y, m_tangent.x };
  }

  // Generates a fixed number of points along the curve.
  std::vector<Vec2f> Generate(int numSamples) 
  {
    std::vector<Vec2f> path;
    path.reserve(numSamples);

    CalcDirTangentNormal();

    for (int i = 0; i <= numSamples; ++i)
    {
      float t = static_cast<float>(i) / numSamples;
      Vec2f pos = CalcPointAtT(t);
      path.push_back(pos);
    }

    return path;
  }

  // Generate one point at t, where t is 0..1 along the curve from
  //  start to end point.
  Vec2f CalcPointAtT(float t) const
  {
      // 1. Base Linear Position (lerp)
      Vec2f currentPos = 
      {
        m_config.startPos.x + m_dir.x * t,
        m_config.startPos.y + m_dir.y * t
      };

      Vec2f offset = { 0.0f, 0.0f };

      // 2. Add Initial Spiral Offset
      if (t <= m_config.spiral.durationT && m_config.spiral.durationT > 0.0f)
      {
        float localT = t / m_config.spiral.durationT;
        
        float directionMult = m_config.spiral.clockwise ? -1.0f : 1.0f;
        float angle = m_config.spiral.startAngleRad + (directionMult * localT * m_config.spiral.turns * 2.0f * M_PI);
        
        float radius = m_config.spiral.expandOutward 
            ? m_config.spiral.maxRadius * localT 
            : m_config.spiral.maxRadius * (1.0f - localT);

        // Smoothly fade out into the main path
        float fadeOut = std::cos(localT * M_PI * 0.5f); 
        radius *= fadeOut;

        offset.x += std::cos(angle) * radius;
        offset.y += std::sin(angle) * radius;
      }

      // 3. Add Loop Offsets (Aligned to Path Direction)
      for (const auto& loop : m_config.loops)
      {
        if (t >= loop.startT && t <= loop.endT)
        {
          float localT = (t - loop.startT) / (loop.endT - loop.startT);
          float angle = localT * loop.rotations * 2.0f * M_PI * (loop.clockwise ? -1.0f : 1.0f);
          
          float envelope = std::sin(localT * M_PI);
          float r = loop.radius * envelope;

          // Project standard circle onto m_tangent/m_normal axes
          offset.x += (m_normal.x * std::cos(angle) + m_tangent.x * std::sin(angle)) * r;
          offset.y += (m_normal.y * std::cos(angle) + m_tangent.y * std::sin(angle)) * r;
        }
      }

      // 4. Add Figure-8 Offsets (Aligned to Path Direction)
      for (const auto& fig8 : m_config.figureEights)
      {
        if (t >= fig8.startT && t <= fig8.endT)
        {
          float localT = (t - fig8.startT) / (fig8.endT - fig8.startT);
          float angle = localT * fig8.rotations * 2.0f * M_PI;
          
          float envelope = std::sin(localT * M_PI);
          
          // Lemniscate math
          float localX = fig8.scale * std::sin(angle);
          float localY = fig8.scale * std::sin(angle) * std::cos(angle);
          
          localX *= envelope;
          localY *= envelope;

          // Project onto m_tangent/m_normal axes
          offset.x += m_tangent.x * localX + m_normal.x * localY;
          offset.y += m_tangent.y * localX + m_normal.y * localY;
        }
      }

      // Combine and store
      currentPos.x += offset.x;
      currentPos.y += offset.y;
      return currentPos;
  }
};
}

