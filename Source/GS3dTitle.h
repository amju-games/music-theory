#pragma once

#include <Singleton.h>
#include "GSBase3d.h"

namespace Amju
{
class GS3dTitle : public GSBase3d
{
public:
  GS3dTitle();

  void Update() override;
  void OnActive() override;
  void OnDeactive() override;

protected:
};

using TheGS3dTitle = Singleton<GS3dTitle>;
}
