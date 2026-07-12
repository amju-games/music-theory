#pragma once

#include <string>
#include <Vec3.h>
#include <Matrix.h>

namespace Amju
{
class GameObject;

std::string Describe(const GameObject* go);

std::string Describe(const Vec3f& v);

std::string Describe(const Matrix& mat);

}
