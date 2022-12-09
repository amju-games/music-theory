// * Amjula Music Theory *
// (c) Copyright Jason Colman 2017-2018
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Attachment.h"

// * Curve *
// Attached to two glyphs, creates a curve between them.
// Used for phrase/slur and ties
class Curve : public Attachment
{
public:
  virtual std::string ToString() const;
  void SetOverOrUnder(bool overNotUnder) { m_overNotUnder = overNotUnder; }

protected:
  // Does the curve form a 'U' (happy smile) shape, or an 'n' (sad face) shape?
  bool m_overNotUnder = true;
};
