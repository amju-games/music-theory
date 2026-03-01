// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <memory>
#include <vector>
#include "StemDir.h"
#include "vec2.h"

class Beam;
struct Glyph;

// * BeamGroup *
// Identifies sequences of beamable notes; creates Beams, sets stems.
// The sequences are represented by indices into the vector of
//  glyphs owned by the bar which we are beaming.
class BeamGroup
{
public:
  BeamGroup() = default;

  // Set index range of glyphs that are members of this beam group.
  // Range is [first, last).
  BeamGroup(int first, int last) : m_first(first), m_last(last) {}

  // Get num glyph members of group
  int GetNumMembers() const { return m_last - m_first; }

  // Decide which way stems of member notes/chords should go;
  //  equivalently, whether the beam group goes above or below the notes.
  void DecideStemDirections(
    std::vector<std::unique_ptr<Glyph>>& glyphs);

  // Set dir of stems of all members: used to override the 
  //  decision made above.
  void SetStemDirection(StemDir dir) { m_stemDir = dir; }

  // Get direction of stem for all notes/chords in this group.
  StemDir GetStemDirection() const { return m_stemDir; }

  // Calc y-coords for end member stems. Units are stave lines,
  //  for testability -- then we can convert to actual coords
  //  when we create quads.
  std::pair<int, int> CalcYStaveLinesAtEnds(
    std::vector<std::unique_ptr<Glyph>>& glyphs);

  // Add all beams in this beam group to the given vector of Beams.
  // We use the glyphs to get x-coord, etc. 
  void AddBeams(std::vector<std::unique_ptr<Beam>>& beams,
    const std::vector<std::unique_ptr<Glyph>>& glyphs);

private:

  // Convert y-coord in 'stave line space' into coord
  //  ready for rendering.
  float ConvertY(float yStaveCoord) const;

  // Render a beam segment: at level (q = 1, qq = 2, etc), from xi to xj,
  //  adding a Beam to the given vector.
  void RenderBeamSegment(
    int level, // beam level
    float xi, float xj, // render beam between xi and xj
    float x1, float x2, // primary beam leftmost and rightmost x
    std::vector<std::unique_ptr<Beam>>& beams);

private:
  StemDir m_stemDir = StemDir::NONE;

  // Indices into glyph vector, [first, last) range. 
  int m_first = -1;
  int m_last = -1;

  // Stave line (i.e. y coord) for start and end of the primary beam,
  //  (which by definition spans the whole beam group).
  // Set by CalcYStaveLinesAtEnds. Used to calc position and gradient
  //  of all beam end pairs.
  std::pair<int, int> m_primaryStaveLines;
};

// Find the beam groups in the given sequence of glyphs.
// This will be runs of contiguous beamable notes/chords.
std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs);

