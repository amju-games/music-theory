// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include "Beam.h"

// * Flag *
// A flag is a beam that only attaches to one stem, rather than two.
// E.g. you would see a flag in a dotted quaver + semiquaver beamed pair.
struct Flag : public Beam
{
  Flag(BeamLevel level_, int left_, int right_, bool stemDir_) :
    Beam(level_, left_, right_),
    stemLeftNotRight(stemDir_)
  {
  }

  std::string ToString() const override;

  // Stem to which the flag is attached is on the left (true) or
  //  right (false).
  bool stemLeftNotRight = true;
};
