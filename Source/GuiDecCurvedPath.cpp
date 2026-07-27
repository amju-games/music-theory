#include <LoadVec2.h>
#include <StringUtils.h>
#include "GuiDecCurvedPath.h"

namespace Amju
{
const char* GuiDecCurvedPath::NAME = "curved-path";

bool GuiDecCurvedPath::Load(File* f) 
{
  // Load curved path config  
  if (!LoadPathConfig(f))
  {
    return false;
  }

  // Set initial pos, backwards or forwards
  const PathConfig& pathConfig = GetPathConfig();
  SetLocalPos(pathConfig.backwards ? pathConfig.endPos : pathConfig.startPos);

  // Load descendants
  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  SetName("curved-path-for-" + GetChild()->GetName());

  return true;
}

bool GuiDecCurvedPath::Save(File* f) 
{
  if (!f->Write(GetTypeName()))
  {
    return false;
  }

  // Save curved path config
  if (!SavePathConfig(f))
  {
    return false;
  }

  return GetChild()->Save(f);
}

void GuiDecCurvedPath::Animate(float animValue) 
{
  Vec2f pos = CalcPointAtT(animValue);
  SetLocalPos(pos);

  AnimateChildren(animValue);
}
}

