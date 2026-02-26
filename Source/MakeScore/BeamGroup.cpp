// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "BeamGroup.h"
#include "Glyph.h"

void BeamGroup::AddNote(Glyph* g)
{
}
 
void BeamGroup::CalcPos()
{
}
 
std::string BeamGroup::ToString()
{
  return ""; 
}

std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs)
{
std::cout << "Finding beam groups....\n";
  std::vector<BeamGroup> res;
  int groupStart = -1;
  const auto numGlyphs = glyphs.size();
  for (int i = 0; i < numGlyphs; i++)
  {
std::cout << "i: " << i << " groupStart: " << groupStart << "\n";
    if (glyphs[i]->IsBeamable())
    {
std::cout << "Glyph " << i << " is beamable\n";
      if (groupStart == -1)
      {
        groupStart = i;
      }
    }
    else
    {
std::cout << "Glyph " << i << " is NOT beamable\n";
      if (groupStart != -1)
      {
        if (groupStart < (i - 1))
        {
          std::cout << "Found a beam group, from element " << groupStart << " to " << i - 1 << "\n";
          res.emplace_back(BeamGroup());
        }
        groupStart = -1;
      } 
    }
  }
  if (groupStart != -1)
  {
    if (groupStart < (numGlyphs - 1))
    {
      std::cout << "Found a beam group, from element " << groupStart << " to " << numGlyphs - 1 << "\n";
      res.emplace_back(BeamGroup());
    }
  }
  return res;
}
