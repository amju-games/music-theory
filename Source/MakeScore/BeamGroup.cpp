// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <algorithm>
#include <iostream>
#include "Beam.h"
#include "BeamGroup.h"
#include "Glyph.h"
#include "NoteAndChordBase.h"

//#define BEAM_GROUP_DEBUG

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

// Calc y-position of ends of a primary beam, grouping all the given 
//  glyphs.
// The return value is (left y-coord, right y-coord), in stave line
//  coord system.
std::pair<int, int> BeamGroup::CalcYStaveLinesAtEnds(
  std::vector<std::unique_ptr<Glyph>>& glyphs) 
{
#ifdef BEAM_GROUP_DEBUG
std::cout << "Calc Y StaveLinesAtEnds....\n";
#endif // BEAM_GROUP_DEBUG
  
  const int MIN_STEM_H = 6;

  // This one is too big, it should be BEAM_THICKNESS + BEAM_GAP,
  //  but longer stems maybe look better?
  const float BEAM_UNIT = 1.5f; // Height of one beam + one gap

  const float MAX_RISE = 2.f; // limit beam gradient: this is max height diff.
  
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
      n->SetStemLength(actualH * .5f); // is 2x length we want

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

bool IsNoteOnBeamBreak(
  TimeValue noteStartTime,
  TimeValue barStartTime,
  TimeSig timeSig) 
{
  // Local time within the bar
  TimeValue t = noteStartTime - barStartTime;
  
  // Floating point epsilon check
  auto isAt = [&](TimeValue target) { 
    return std::abs(t - target) < 0.001f; 
  };

  switch (timeSig) 
  {
    case TimeSig::TIME_SIG_NONE:
      // No breaks if no time sig; this is for testing and any other
      //  situation where we don't want breaks.
      return false;

    case TimeSig::TIME_SIG_FOUR_FOUR:
    case TimeSig::TIME_SIG_COMMON:
      // Primary break is at the middle of the bar (Beat 3)
      return isAt(2.0f);

    case TimeSig::TIME_SIG_TWO_FOUR:
    case TimeSig::TIME_SIG_CUT_COMMON:
      // Break on every crotchet beat
      return isAt(1.0f);

    case TimeSig::TIME_SIG_THREE_FOUR:
      // Usually beams aren't joined across any crotchet beats in 3/4
      return isAt(1.0f) || isAt(2.0f);

    case TimeSig::TIME_SIG_SIX_FOUR:
      // Split at middle of bar
      return isAt(3.0f);

    case TimeSig::TIME_SIG_SIX_EIGHT:
      // Two beats of 1.5 crotchets (three quavers) each
      return isAt(1.5f);

    case TimeSig::TIME_SIG_NINE_EIGHT:
      // Three beats of 1.5 crotchets each
      return isAt(1.5f) || isAt(3.0f);

    case TimeSig::TIME_SIG_TWELVE_EIGHT:
      // Four beats of 1.5 crotchets each
      return isAt(1.5f) || isAt(3.0f) || isAt(4.5f);

    default:
      // For unknown signatures, default to breaking on every whole crotchet
      return std::abs(fmod(t, 1.0f)) < 0.001f && t > 0.001f;
  }
}

std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs,
  TimeValue barStartTime, // start time of first beat of bar
  TimeSig timeSig)
{
#ifdef BEAM_GROUP_DEBUG
std::cout << "Finding beam groups....\n";
#endif //  BEAM_GROUP_DEBUG
  std::vector<BeamGroup> res;
  int groupStart = -1;
  const int numGlyphs = static_cast<int>(glyphs.size());
  for (int i = 0; i < numGlyphs; i++)
  {
    // Break beam if we are on a new beat, (which depends on time sig).
    bool beamBreak = IsNoteOnBeamBreak(
      glyphs[i]->GetTimes().GetStartTimeValue(), // note start time
      barStartTime, timeSig);

    // BREAK_BEAM_ON_TIE
    // Also break beam if note glyph is tied to another note.
    beamBreak |= ((glyphs[i]->GetTieLeft() != nullptr) ||
                  (glyphs[i]->GetTieRight() != nullptr)); 

    if (beamBreak && groupStart != -1)
    {
      // Major beat breaks beam we were making, so add it and reset counter.
      if (i > (groupStart + 1)) // don't add single note as a beam group
      {
        res.emplace_back(BeamGroup(groupStart, i));
      }
      groupStart = -1;
    }

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
 
  const float BEAM_PLUS_GAP_HEIGHT = 1.5f; // in stave line coord system
 
  float offset = (level - 1) * BEAM_PLUS_GAP_HEIGHT;
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

  const float y1 = static_cast<float>(m_primaryStaveLines.first);
  const float y2 = static_cast<float>(m_primaryStaveLines.second);

  // Left and right end points of line seg
  vec2 left(
    xi + xOff,
    ConvertY(GetBeamY(xi, x1, y1, x2, y2, level, m_stemDir)));

  vec2 right(
    xj + xOff,
    ConvertY(GetBeamY(xj, x1, y1, x2, y2, level, m_stemDir)));

  beams.emplace_back(std::make_unique<Beam>(left, right));
}

// Return true if glyph (we assume note or chord) is on a beat.
static bool IsOnBeat(const std::unique_ptr<Glyph>& g)
{
  // Get (non-normalised) start time of note
  const TimeValue t = g->GetTimes().GetStartTimeValue();
  // Beats are whole numbers; get distance from nearest whole number
  //  and compare with epsilon.
  return std::abs(t - std::round(t)) < 0.001f;
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
        (dynamic_cast<NoteAndChordBase*>(glyphs[i+1].get())->
          GetBeamLevel() >= level);

      bool prevHasLevel = (i > m_first) && 
        (dynamic_cast<NoteAndChordBase*>(glyphs[i-1].get())->
          GetBeamLevel() >= level);

      bool isNextOnBeat = (i + 1 < m_last) && IsOnBeat(glyphs[i+1]);
      
      float x_i = glyphs[i]->GetPos().x;
      
      if (currentHasLevel && nextHasLevel) 
      {
        // Skip rendering this segment if next note is on beat.
        if (!isNextOnBeat)
        {
          // Draw a full secondary beam segment from i to i+1
          float x_next = glyphs[i+1]->GetPos().x;
          RenderBeamSegment(level, x_i, x_next, x1, x2, beams);
        }
      } 
      else if (currentHasLevel) 
      {
        // Draw a broken beam (stub)?
        bool yesDrawStub = !prevHasLevel; // neither neighbour at same level
        // TODO Perhaps also draw stub if this note is dotted. 
        if (yesDrawStub)
        {
          const float stubW = NOTE_HEAD_WIDTH * .8f;
          float stubX = CalcStubEndPosX(i, level, stubW, glyphs); 
          RenderBeamSegment(level, x_i, stubX, x1, x2, beams);
        }
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

float BeamGroup::CalcStubEndPosX(
  int i, [[maybe_unused]]int level, float stubLen, 
  const std::vector<std::unique_ptr<Glyph>>& glyphs) 
{
  float currentX = glyphs[i]->GetPos().x;

  // Basic heuristic: 
  // If there's a neighbor to the left within the same BeamGroup, point left.
  // Otherwise, point right.
  
  bool hasLeftNeighbour = (i > m_first);
  bool hasRightNeighbour = (i < m_last - 1);

  // If we have neighbours on both sides, point to the neighbour with
  //  longer duration.

  if (hasLeftNeighbour && hasRightNeighbour &&
      glyphs[i-1]->GetTimes().GetTimeValue() > glyphs[i+1]->GetTimes().GetTimeValue())
  {
    // Left neighbour has larger duration than right: point left
    return currentX - stubLen;
  }

  // In a 16th-note stub scenario (like 8th + 16th):
  // The 16th is at the end (idx == m_last - 1), so it must point left.
  if (hasLeftNeighbour && !hasRightNeighbour) 
  {
    return currentX - stubLen;
  }
  
  // If it's at the start of the group, point right.
  return currentX + stubLen;
}

