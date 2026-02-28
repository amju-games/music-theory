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
     Once end points are decided, set length of each stem.
     First do end glyphs, then interpolate to do mid glyphs.

 - Rendering:
   - Generate quad for primary beam - done!!
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

// Get the stave line of a note or chord.
// We need to pass in the stem direction because for a chord,
//  we choose either the top note (if stem goes up), or bottom
//  note (if stem goes down).
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
  
  const int MIN_STEM_H = 6;
  const float BEAM_UNIT = .5f; // Height of one beam + one gap
  const float MAX_RISE = 2.f; // limit gradient
  
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

  // Clamp the initial slope so it's not too crazy
  float rise = y2 - y1;
  if (std::abs(rise) > MAX_RISE) 
  {
    float direction = (rise > 0) ? 1.0f : -1.0f;
    y2 = y1 + (MAX_RISE * direction);
  }

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
      n->SetStemLength(actualH * .5f); // is 2x length?? 

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

  // Return y-coords of end points, in stave line space.
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
      n->SetIsBeamed();
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

// Helper to get the Y of a specific beam level at a specific X
static float GetBeamY(float x, float x1, float y1, float x2, float y2, 
  int level, StemDir dir) 
{
  float t = (x - x1) / (x2 - x1);
  float primaryY = y1 + t * (y2 - y1);
  
  float offset = (level - 1) * 1.0f; // 1.0 is the beam + gap height
  // Secondary beams are always "inside" the primary beam (closer to notehead)
  return (dir == StemDir::UP) ? (primaryY - offset) : (primaryY + offset);
}

void BeamGroup::RenderBeamSegment(
  int level, // beam level
  float xi, float xj, // render beam between xi and xj
  float x1, float x2, // primary beam leftmost and rightmost x
  std::vector<std::unique_ptr<Beam>>& beams)
{
  float xOff = (m_stemDir == StemDir::UP ? STEM_UP_X_OFFSET : STEM_DOWN_X_OFFSET);

  const float y1 = m_primaryStaveLines.first;
  const float y2 = m_primaryStaveLines.second;

  // Left and right end points of line seg
  vec2 left(
    xi + xOff,
    ConvertY(GetBeamY(xi, x1, y1, x2, y2, level, m_stemDir)));

  vec2 right(
    xj + xOff,
    ConvertY(GetBeamY(xj, x1, y1, x2, y2, level, m_stemDir)));

  beams.emplace_back(std::make_unique<Beam>(left, right));
}

void BeamGroup::AddBeams(std::vector<std::unique_ptr<Beam>>& beams,
 const std::vector<std::unique_ptr<Glyph>>& glyphs)
{
  // Left and right x extents
  const float x1 = glyphs[m_first]->GetPos().x;
  const float x2 = glyphs[m_last - 1]->GetPos().x;

  // Add primary beam
  const int PRIMARY = 1;
  RenderBeamSegment(PRIMARY, x1, x2, x1, x2, beams);
/*
  // Left and right end points of line seg
  vec2 left(
    glyphs[m_first]->GetPos().x + xOff, 
    ConvertY(m_primaryStaveLines.first));

  vec2 right(
    glyphs[m_last - 1]->GetPos().x + xOff, 
    ConvertY(m_primaryStaveLines.second));

  beams.emplace_back(std::make_unique<Beam>(left, right));
*/
  // Add secondary beam line segs. Traverse the glyphs in our range [first, last)
  //  and create beams using primary beam to get position and gradient, and
  //  beam level from each glyph to decide the number and type of secondary beams.

  const int maxLevel = 3; // TODO QQQ increase for QQQQ support etc
  for (int level = 2; level <= maxLevel; ++level) 
  {
    for (int i = m_first; i < m_last; ++i) 
    {
      bool currentHasLevel = dynamic_cast<NoteAndChordBase*>(glyphs[i].get())->
        GetBeamLevel() >= level;

      bool nextHasLevel = (i + 1 < m_last) && 
        (dynamic_cast<NoteAndChordBase*>(glyphs[i+1].get())->GetBeamLevel() >= level);
      
      float x_i = glyphs[i]->GetPos().x;
      
      if (currentHasLevel && nextHasLevel) 
      {
        // Draw a full secondary beam segment from i to i+1
        float x_next = glyphs[i+1]->GetPos().x;
        RenderBeamSegment(level, x_i, x_next, x1, x2, beams);
      } 
      else if (currentHasLevel) 
      {
        // Draw a broken beam (stub)
        // Direction logic: if i is the start of the group, point right. 
        // If i is the end, point left.
        const bool isStart = (i == m_first);
        const float stubW = NOTE_HEAD_WIDTH * .45f;
        float stubX = x_i + stubW * (isStart ? 1.0f : -1.0f); 
        RenderBeamSegment(level, x_i, stubX, x1, x2, beams);
      }
    }
  }
}

float BeamGroup::ConvertY(float yStaveCoord) const
{
  // convert to 'render space' y-coord
  float y = yStaveCoord * STAVE_LINE_GAP * .5f;
  return y; 
}

