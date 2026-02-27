// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm>
#include <iostream>
#include <stdexcept> //?
#include "Beam.h"
#include "BeamGroup.h"
#include "Glyph.h"
#include "NoteAndChordBase.h"

//#define BEAM_GROUP_DEBUG

/* What we need to do for beams:

 - Identify runs of beamable glyphs -- done!

 - Break up runs depending on beat/position in bar 
   Two cases:
   1. For runs of q, break into 2 runs at the middle of the bar
   2. For <q, only the primary beam can span the middle of the bar.
      So the beam group is intact, but beam level between mid-point
      glyphs is 1.

 - For one beam group, decide up/down / Above/below position of beams 
    <==>  same as deciding stem dir for all stems in beam group.
     take majority decision of the glyphs stem dirs.
  -- done!

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

 - Rendering:
   - Generate quad for primary beam
   - Generate quads for secondary beams and broken beams
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

static int GetStaveLine(std::unique_ptr<Glyph>& g, StemDir dir)
{
  auto n = dynamic_cast<NoteAndChordBase*>(g.get());
  assert(n);
  
  return n->GetStaveLineForBeam(dir);
}

float CalcStaveLineToBeamDistance(vec2 p, std::pair<vec2, vec2> beam) 
{
    float x1 = beam.first.x;
    float x2 = beam.second.x;
    float y1 = beam.first.y;
    float y2 = beam.second.y;

    // Ensure x1 is the leftmost point for easier calculation
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    // Check if the point is horizontally within the beam's span
    if (p.x < x1 || p.x > x2) {
        // Depending on your needs, you might return 0, infinity, 
        // or the distance to the nearest endpoint.
        return -1.0f; 
    }

    // Handle perfectly vertical beams to avoid division by zero
    if (std::abs(x1 - x2) < 1e-6f) {
        return std::abs(p.y - std::min(y1, y2));
    }

    // Linear interpolation: find the y-coordinate on the beam at p.x
    // formula: y = y1 + (p.x - x1) * (y2 - y1) / (x2 - x1)
    float beamYAtP = y1 + (p.x - x1) * (y2 - y1) / (x2 - x1);

    // Return the absolute vertical distance
    return std::abs(beamYAtP - p.y);
}

std::pair<int, int> BeamGroup::CalcYStaveLinesAtEnds(
  std::vector<std::unique_ptr<Glyph>>& glyphs) 
{
#ifdef BEAM_GROUP_DEBUG
std::cout << "Calc Y StaveLinesAtEnds....\n";
#endif // BEAM_GROUP_DEBUG
  
  const int MIN_STEM_H = 4;
  const float BEAM_UNIT = 1.0f; // Height of one beam + one gap
  
  // 1. Determine the "thickest" part of the beam stack
  int maxLevel = 1;
  for (int i = m_first; i < m_last; ++i) 
  {
    auto n = dynamic_cast<NoteAndChordBase*>(glyphs[i].get());
    maxLevel = std::max(maxLevel, n->GetBeamLevel());
  }

  // Secondary beams sit inside the primary. 
  // We need the primary beam to be further away to accommodate them.
  const float stackOffset = (maxLevel - 1) * BEAM_UNIT;
  const float requiredDist = static_cast<float>(MIN_STEM_H) + stackOffset;

#ifdef BEAM_GROUP_DEBUG
std::cout << "MaxLevel: " << maxLevel 
  << "  required dist: " << requiredDist  << "\n";
#endif // BEAM_GROUP_DEBUG

  int s1 = GetStaveLine(glyphs[m_first], m_stemDir);
  int s2 = GetStaveLine(glyphs[m_last - 1], m_stemDir);
  int dh = (m_stemDir == StemDir::DOWN) ? -1 : 1;

  // Initial ideal beam positions
  float y1 = static_cast<float>(s1 + (MIN_STEM_H * dh));
  float y2 = static_cast<float>(s2 + (MIN_STEM_H * dh));
#ifdef BEAM_GROUP_DEBUG
std::cout << "Starting! y1: " << y1 << " y2: " << y2 << "\n";
#endif // BEAM_GROUP_DEBUG

  float x1 = glyphs[m_first]->GetPos().x;
  float x2 = glyphs[m_last - 1]->GetPos().x;

  // 2. The "Intelligent Lift"
  // Find the note that violates the requiredDist the most and push the whole beam
  bool adjustmentNeeded = true;
  while (adjustmentNeeded) 
  {
    adjustmentNeeded = false;
    float maxViolation = 0.0f;

    for (int i = m_first; i < m_last; ++i) 
    {
      auto n = dynamic_cast<NoteAndChordBase*>(glyphs[i].get());
      float x = n->GetPos().x;
      float noteY = static_cast<float>(GetStaveLine(glyphs[i], m_stemDir));
      
      // Interpolate current beam position at x
      float t = (x2 == x1) ? 0.0f : (x - x1) / (x2 - x1);
      float currentBeamY = y1 + t * (y2 - y1);
#ifdef BEAM_GROUP_DEBUG
std::cout << "i: " << i << "  noteY: " << noteY 
  << "  currentBeamY: " << currentBeamY << " (t: " << t << ")\n";
#endif // BEAM_GROUP_DEBUG

      // Signed distance: positive if stem is long enough
      float actualH = 
        (m_stemDir == StemDir::UP) ? (currentBeamY - noteY) : 
                                     (noteY - currentBeamY);

      // Set stem length on glyph
      // TODO
      //n->SetStemLength(actualH);  

      if (actualH < requiredDist) 
      {
        maxViolation = std::max(maxViolation, requiredDist - actualH);
        adjustmentNeeded = true;
#ifdef BEAM_GROUP_DEBUG
std::cout << "  actual H: " << actualH << "  max violation: " << maxViolation << "\n";
#endif // BEAM_GROUP_DEBUG
      }
    }

    if (adjustmentNeeded) {
#ifdef BEAM_GROUP_DEBUG
std::cout << "max violation: " << maxViolation << "\n";
#endif // BEAM_GROUP_DEBUG
      y1 += maxViolation * dh;
      y2 += maxViolation * dh;
#ifdef BEAM_GROUP_DEBUG
std::cout << "New values for y1 and y2: y1: " << y1 << " y2: " << y2 << "\n";
#endif // BEAM_GROUP_DEBUG
    }
  }

#ifdef BEAM_GROUP_DEBUG
std::cout << "Finished! y1: " << y1 << " y2: " << y2 << "\n";
#endif // BEAM_GROUP_DEBUG

  m_primaryStaveLines = 
    { static_cast<int>(std::round(y1)), static_cast<int>(std::round(y2)) };
  return m_primaryStaveLines;
}

/* old, human-written version

std::pair<int, int> BeamGroup::CalcYStaveLinesAtEnds(
  std::vector<std::unique_ptr<Glyph>>& glyphs)
{
  // For start and end member notes/chords:
  //  - work out direction to go, up or down
  //  - add stem height to y-pos of note
  //  - That's it! Not really - iterate until all stems meet minimum length.

  // +ve y is UP

  // We will set the beam end points so all stems are at least this long.
  const int MIN_STEM_H = 4;

  int h = MIN_STEM_H; // We start with stem this height at both ends
  if (m_stemDir == StemDir::DOWN) h = -h;
  int h1 = h;
  int h2 = h;
  int s1 = GetStaveLine(glyphs[m_first], m_stemDir);
  int s2 = GetStaveLine(glyphs[m_last - 1], m_stemDir); 

  // X coords of end glyphs
  float x1 = glyphs[m_first]->GetPos().x;
  float x2 = glyphs[m_last - 1]->GetPos().x;
 
  std::pair<int, int> res;
  bool allHeightsOk = true;

  // Check height of intervening stems, adjust h and iterate until
  //  all heights are big enough.
  // Amount to adjust each iteration
  int dh = (m_stemDir == StemDir::DOWN) ? -1 : 1;
  do
  {
    res = { s1 + h1, s2 + h2 };
    
    // Check heights of intervening stems
    for (int i = m_first + 1; i < (m_last - 1); i++)
    {
      allHeightsOk = true;
      int staveLine = GetStaveLine(glyphs[i], m_stemDir);

      // X coord of this intervening glyph
      float x = glyphs[i]->GetPos().x;

      // Get vertical distance from this intervening glyph to the beam.
      float stemH = CalcStaveLineToBeamDistance(
        vec2(x, staveLine), 
        { {x1, static_cast<float>(res.first)}, 
          {x2, static_cast<float>(res.second)} } );

      if (fabs(stemH) < MIN_STEM_H)
      {
        allHeightsOk = false;
        h1 += dh; // TODO adjust more intelligently
        h2 += dh; 
        break; // don't consider any more stems: that's only ok if we are
               //  extending both ends the same amount.
      }
    }
  } while (!allHeightsOk);

  return res;
}
*/

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

void BeamGroup::AddBeams(std::vector<std::unique_ptr<Beam>>& beams)
{
  // Add primary beam
  // left and right are vec2s in final coord space, right?
  // Or do we do that conversion here?
  // OR do we do it when we make quads inside Beam??

  //beams.emplace_back(std::make_unique<Beam>(left, right);
}


