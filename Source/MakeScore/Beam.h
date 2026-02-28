// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"
#include "Utils.h"

class Beam : public IGlyph
{
public:
  Beam() = default;
  Beam(const vec2& left, const vec2& right) : m_left(left), m_right(right) {}

  std::string ToString() const override;

  std::string CommentString() const override;

protected:
  // Left and right endpoints of beam, in final coordinate space
  //  (not stave line space!) - but excluding final positioning and scaling, 
  //  which we apply in ToString.
  vec2 m_left, m_right;
};

