#pragma once

#include "CurvedPath.h"

namespace Amju
{
class File;

// * CurvedPathMixin *
// Generate curved path for GUI or Scene node.
class CurvedPathMixin
{
public:
  virtual ~CurvedPathMixin() = default;

  // Load path config, not actual points on the curve.
  bool LoadPathConfig(File*);
  bool SavePathConfig(File*) const;

  // Calc point along curve at t (0..1)
  Vec2f CalcPointAtT(float t) const;

  // Get/set path config, which defines the path.
  const PathConfig& GetPathConfig() const { return m_curvedPathGenerator.GetConfig(); }
  void SetPathConfig(const PathConfig& pc) { m_curvedPathGenerator.SetConfig(pc); }

private:
  CurvedPathGenerator m_curvedPathGenerator;
};
}

