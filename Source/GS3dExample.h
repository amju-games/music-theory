#pragma once

#include "GSBase3d.h"

namespace Amju
{
class GS3dExample : public GSBase3d
{
public:
  GS3dExample();
  void Draw() override;
};

using TheGS3dExample = Singleton<GS3dExample>;
}

