// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "IGlyph.h"
#include "Utils.h"

enum class BeamLevel
{
  BEAM_LEVEL_1 = 0,
  BEAM_LEVEL_2 = 1,
};

float GetHeight(BeamLevel bl);

struct Beam : public IGlyph
{
  Beam() = default;
  Beam(BeamLevel level_, int left_, int right_) :
    level(level_), left(left_), right(right_) {}

  static constexpr float BEAM_HEIGHT = 0.06f;
  static constexpr float BEAM_Y_MIN = 0.770f;

  std::string ToString() const override;

  // Set 'parent' beam for flag or 2nd/3rd beam, so we can take the
  //  gradient from the parent.
  // OR Maybe point to owning BeamGroup instead?
  void SetParentBeam(Beam* p) { parent = p; }

  BeamLevel level = BeamLevel::BEAM_LEVEL_1;
  int left = 0;
  int right = 0;
  float xmin = 0;
  float xmax = 0;

  // note type, so we know where to offset the quad corners
  bool upNotDown = true;

  float gradient = 0;

  // For flags and second/third beams, this is the first, primary
  //  beam, which has the gradient calculated on it. We take the same
  //  gradient for decendants.
  // OR Maybe point to owning BeamGroup instead?
  Beam* parent = nullptr;
};

