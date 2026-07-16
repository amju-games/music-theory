#pragma once

#include <Vec3.h>

namespace Amju
{
// Convenience function: get dir in degrees for vector: y-component is
//  ignores, as this is intended for rotating around the y axis.
float GetAngleDegsFromVec(const Vec3f& vec);

// Convenience function: get Vec3 for given heading in degrees. The
//  heading is in x-z plane: y component is zero.
Vec3f GetVecFromAngleDegs(float degs);
}

