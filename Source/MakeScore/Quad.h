// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include <array>
#include "IGlyph.h"

// * Quad *
// Base class?
// Used for stems, beams, ledger lines, etc.
class Quad : public IGlyph
{
public:
  Quad() = default;

  // Construct axis-aligned quad: set min and max corners
  Quad(float xmin, float ymin, float xmax, float ymax);

  // TODO Construct arbitrary quad - CCW vertices?
  Quad(float cornerXY[8]);

  std::string ToString() const override;

protected:
  std::array<float, 8> m_cornerXY;
};

