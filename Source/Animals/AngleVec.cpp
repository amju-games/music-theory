#include <cmath>
#include <DegRad.h>
#include "AngleVec.h"

namespace Amju
{
// Convenience functions
float GetAngleDegsFromVec(const Vec3f& vec)
{
  float degs = RadToDeg(std::atan2(vec.x, vec.z));
  return degs;
}

Vec3f GetVecFromAngleDegs(float degs)
{
  const float rad = DegToRad(degs);
  Vec3f vec(std::sin(rad), 0, std::cos(rad));
  return vec;
}
}

