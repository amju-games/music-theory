#pragma once

#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
// * GSShowNewLevelNum *
// Shown when we travel between levels, when we enter a lift.
class GSShowNewLevelNum : public GSBase
{
public:
  GSShowNewLevelNum();
  void OnActive() override;
};

using TheGSShowNewLevelNum = Singleton<GSShowNewLevelNum>;

}

