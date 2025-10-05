// * Amjula music theory * (c) Copyright 2024 Juliet Colman

#ifdef RANGES_AVAILABLE // no <ranges> on Mac OS 11 ("Big Sur")
#include <ranges> // get map values into a vector
#endif

#include <File.h>
#include <StringUtils.h>
#include "Palette.h"

namespace Amju
{
Palette::ColourVec Palette::GetColours() const
{
#ifdef RANGES_AVAILABLE // no <ranges> on Mac OS 11 ("Big Sur")

  auto vv = std::views::values(m_colours);
  return ColourVec{ vv.begin(), vv.end() };

#else

  ColourVec result;
  for (const auto& kv : m_colours)
  {
    result.push_back(kv.second);
  }
  return result;

#endif
}

bool Palette::ParseLine(const std::string& s)
{
  auto nameColour = Split(s, '=');
  if (nameColour.size() == 2)
  {
    auto optionalColour = FromHexString(nameColour[1]);
    if (optionalColour)
    {
      m_colours[nameColour[0]] = *optionalColour;
      return true;
    }
  }
  return false;
}

bool Palette::Load(File& f)
{
  // Assume it's just palette entries up to EOF
  std::string s;
  while (f.GetDataLine(&s))
  {
    if (!ParseLine(s))
    {
      f.ReportError("Didn't like this palette line: " + s);
      return false;
    }
  }
  return true;
}

bool Palette::Save(File& f)
{
  return false;
}
}
