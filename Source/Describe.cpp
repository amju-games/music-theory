#include <GameObject.h>
#include <StringUtils.h>
#include <Quaternion.h>
#include <DegRad.h>
#include "Describe.h"

namespace Amju
{
std::string Describe(const GameObject* go)
{
  return std::string(go->GetTypeName()) + " ID: " + ToString(go->GetId());
}

std::string Describe(const Vec3f& v)
{
  return "(" + ToString(v.x, 2) + ", " + ToString(v.y, 2) + ", " + ToString(v.z, 2) + ")";
}

std::string Describe(const Matrix& mat)
{
  // Get translation
  const float* const elems = mat; // yuck, operator const float*
  Vec3f translate(elems[12], elems[13], elems[14]);

  // Get rotation in a readable form
  Quaternion q;
  q.CreateFromMatrix(mat);
  float rads = 0;
  Vec3f axis;
  q.GetAxisAngle(&rads, &axis);
  return "Tr: " + Describe(translate) + 
    " Rot: " + ToString(RadToDeg(rads), 2) + 
    " degs around " + Describe(axis);
}

}

