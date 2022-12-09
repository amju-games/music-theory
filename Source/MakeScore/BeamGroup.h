// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <string>
#include <vector>

class Glyph;

// * BeamGroup *
// Stores sequence of <*>Quavers, which are to be beamed.
// Once all notes are added to the beam group, and their positions
//  set, we can decide on the beam position and gradient, and set
//  the stem lengths on the notes.
class BeamGroup
{
public:
  // Add a note to be beamed to the group: this must be a quaver,
  //  semiquaver, etc.
  void AddNote(Glyph* g);

  // Once final note has been added, call this to calculate beam
  //  positions, note stems.
  void CalcPos();

  // Output quads for beams and stems
  // Not sure if we need this? 
  std::string ToString();

private:
  std::vector<Glyph*> m_notes;
};

