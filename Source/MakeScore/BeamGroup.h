// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <memory>
#include <string>
#include <vector>
#include "StemDir.h"
#include "vec2.h"

class Beam;
struct Glyph;

// * BeamGroup *
// Stores sequence of <*>Quavers, which are to be beamed.
// Once all notes are added to the beam group, and their positions
//  set, we can decide on the beam position and gradient, and set
//  the stem lengths on the notes.
class BeamGroup
{
public:
  BeamGroup() = default;

  // Set index range of glyphs that are members of this beam group.
  // Range is [first, last) like stl iterators etc.
  BeamGroup(int first, int last) : m_first(first), m_last(last) {}

  // Add a note to be beamed to the group: this must be a quaver,
  //  semiquaver, etc.
  void AddNote(Glyph* g);

  // Once final note has been added, call this to calculate beam
  //  positions, note stems.
  void CalcPos();

  // Output quads for beams and stems
  // Not sure if we need this? 
  std::string ToString();

  // Get num glyph members of group
  int GetNumMembers() const { return m_last - m_first; }

  // Decide which way stems of member notes/chords should go,
  //  using 'majority vote'.
  void DecideStemDirections(
    std::vector<std::unique_ptr<Glyph>>& glyphs);

  // Set dir of stems of all members: used to override the 
  //  'majority vote' decision.
  void SetStemDirection(StemDir dir) { m_stemDir = dir; }

  // Get direction of stem for all notes/chords in this group.
  StemDir GetStemDirection() const { return m_stemDir; }

  // Calc y-coords for end member stems. Units are stave lines,
  //  for testability -- then we can convert to actual coords
  //  when we create quads.
  std::pair<int, int> CalcYStaveLinesAtEnds(
    std::vector<std::unique_ptr<Glyph>>& glyphs);

  // Create quad for primary beam -- maybe we could 
  //  break this down a bit.
//  Quad MakePrimaryBeam(
//    std::vector<std::unique_ptr<Glyph>>& glyphs);

  // Add all beams in this beam group to the given vector.
  void AddBeams(std::vector<std::unique_ptr<Beam>>& beams);

private:
  StemDir m_stemDir = StemDir::NONE;

  // Indices into glyph vector, [first, last) range. 
  int m_first = -1;
  int m_last = -1;

  // Stave line (i.e. y coord) for start and end of the primary beam,
  //  (which by definition spans the whole beam group).
  // Set by CalcYStaveLinesAtEnds
  std::pair<int, int> m_primaryStaveLines;
};

using PBeamGroup = std::shared_ptr<BeamGroup>;

// Find the beam groups in the given sequence of glyphs.
// This will be runs of contiguous beamable notes/chords.
std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs);

// Helper function: calc vertical distance from given note pos
//  to a beam defined by its endpoints.
float CalcStaveLineToBeamDistance(
  vec2 p, std::pair<vec2, vec2> beamEnds);

