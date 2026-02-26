// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <iostream>
#include "BeamGroup.h"
#include "Glyph.h"
#include "NoteAndChordBase.h"

/* What we need to do for beams:

 - Identify runs of beamable glyphs -- done

 - Break up runs depending on beat/position in bar 
   Two cases:
   1. For runs of q, break into 2 runs at the middle of the bar
   2. For <q, only the primary beam can span the middle of the bar.
      So the beam group is intact, but beam level between mid-point
      glyphs is 1.

 - For one beam group, decide up/down / Above/below position of beams 
    <==>  same as deciding stem dir for all stems in beam group.
     take majority decision of the glyphs stem dirs.

 - Positioning:
   - Position of primary beam in y at ends of the beam group.
     This might need to be an iterative algo.
     Start with regular length stems at the ends.
     For each mid glyph, calc stem length. If too short, extend the
      y pos at the end closer to the glyph.
      - include the height of the beams/flags too, i.e. stem length
        is regular length + num flags/beams * height of a beam.

     Once end points are decided, set length of each stem.
     First do end glyphs, then interpolate to do mid glyphs.
*/


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

void BeamGroup::DecideStemDirections(
  std::vector<std::unique_ptr<Glyph>>& glyphs)
{
#ifdef BEAM_GROUP_DEBUG
std::cout << "Top of BeamGroup::DecideStemDirections...\n";
#endif //  BEAM_GROUP_DEBUG

  // Get stem dir of each member, go with the majority.
  int stemUpPositive = 0;
  for (int i = m_first; i < m_last; i++)
  {
    auto n = dynamic_cast<NoteAndChordBase*>(glyphs[i].get());
    if (n)
    {
      auto stemDir = n->DecideStemDirection();
#ifdef BEAM_GROUP_DEBUG
std::cout << " Dir for glyph " << i << " is: " << (stemDir == StemDir::UP ? "up" : "down") << "\n";
#endif //  BEAM_GROUP_DEBUG
      if (stemDir == StemDir::UP) 
        stemUpPositive++; 
      else 
        stemUpPositive--;
    }
  }
#ifdef BEAM_GROUP_DEBUG
std::cout << "Beam Group stem dir decision: " << (stemUpPositive >= 0 ? "UP" : "DOWN") << "\n";
#endif //  BEAM_GROUP_DEBUG
  // TODO Dig deeper if zero: are there other criteria we could use?

  // Pass 2: set decided stem dir on all members
  auto dir = (stemUpPositive >= 0 ? StemDir::UP : StemDir::DOWN);

  for (int i = m_first; i < m_last; i++)
  {
    auto n = dynamic_cast<NoteAndChordBase*>(glyphs[i].get());
    if (n)
    {
      n->SetStemDirection(dir);
    }
  } 
  m_stemDir = dir;
}

std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs)
{
#ifdef BEAM_GROUP_DEBUG
std::cout << "Finding beam groups....\n";
#endif //  BEAM_GROUP_DEBUG
  std::vector<BeamGroup> res;
  int groupStart = -1;
  const auto numGlyphs = glyphs.size();
  for (int i = 0; i < numGlyphs; i++)
  {
#ifdef BEAM_GROUP_DEBUG
std::cout << "i: " << i << " groupStart: " << groupStart << "\n";
#endif //  BEAM_GROUP_DEBUG
    if (glyphs[i]->IsBeamable())
    {
#ifdef BEAM_GROUP_DEBUG
std::cout << "Glyph " << i << " is beamable\n";
#endif //  BEAM_GROUP_DEBUG
      if (groupStart == -1)
      {
        groupStart = i;
      }
    }
    else
    {
#ifdef BEAM_GROUP_DEBUG
std::cout << "Glyph " << i << " is NOT beamable\n";
#endif //  BEAM_GROUP_DEBUG
      if (groupStart != -1)
      {
        if (groupStart < (i - 1))
        {
#ifdef BEAM_GROUP_DEBUG
          std::cout << "Found a beam group, from element " << groupStart << " to " << i - 1 << "\n";
#endif //  BEAM_GROUP_DEBUG
          res.emplace_back(BeamGroup(groupStart, i));
        }
        groupStart = -1;
      } 
    }
  }
  if (groupStart != -1)
  {
    if (groupStart < (numGlyphs - 1))
    {
#ifdef BEAM_GROUP_DEBUG
      std::cout << "Found a beam group, from element " << groupStart << " to " << numGlyphs - 1 << "\n";
#endif //  BEAM_GROUP_DEBUG
      res.emplace_back(BeamGroup(groupStart, numGlyphs));
    }
  }
  return res;
}
