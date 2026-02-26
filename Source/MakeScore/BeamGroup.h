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

  // Get direction of stem for all notes/chords in this group.
  StemDir GetStemDirection() const { return m_stemDir; }

  // Get num glyph members of group
  int GetNumMembers() const { return m_last - m_first; }

  void DecideStemDirections(
    std::vector<std::unique_ptr<Glyph>>& glyphs);

private:
  // No: the notes and chords point to this beam group, with a shared
  //  ptr -- so the members of the beam group own it?!
//  std::vector<Glyph*> m_notes;

  StemDir m_stemDir = StemDir::NONE;

  // Indices into glyph vector, [first, last) range. 
  int m_first = -1;
  int m_last = -1;
};

using PBeamGroup = std::shared_ptr<BeamGroup>;

// Find the beam groups in the given sequence of glyphs.
// This will be runs of contiguous beamable notes/chords.
std::vector<BeamGroup> FindBeamGroups(
  const std::vector<std::unique_ptr<Glyph>>& glyphs);

