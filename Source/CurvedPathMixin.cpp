#include <File.h>
#include <LoadVec2.h>
#include <StringUtils.h>
#include "CurvedPathMixin.h"

namespace Amju
{
bool CurvedPathMixin::LoadPathConfig(File* f)
{
  // Load start, end coords 
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected data for curved path.");
    return false;
  }

  Strings strs = Split(s, ';');
  if (strs.size() != 2)
  {
    f->ReportError("Expected position pair for curved path.");
    return false;
  }

  PathConfig pathConfig;

  ToVec2(strs[0], &pathConfig.startPos);
  ToVec2(strs[1], &pathConfig.endPos);

  // TODO Load the members... or set procedurally

  SetPathConfig(pathConfig);

  return true;
}

bool CurvedPathMixin::SavePathConfig(File* f) const
{
  // TODO Save curved path -- not a priority
  return false;
}

Vec2f CurvedPathMixin::CalcPointAtT(float t) const
{
  return m_curvedPathGenerator.CalcPointAtT(t);
}
}

