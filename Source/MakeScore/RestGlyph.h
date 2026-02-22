// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#pragma once

#include "Glyph.h"

// Parsing: return true if token is a rest which should be added to the
//  score immediately.
bool IsImmediateRest(const std::string& token);

// `R` input token means whole bar rest
bool IsWholeBarRest(const std::string& token);

// Parsing: return true if token is a deferred rest, i.e. we should add
//  a rest to the score for each subsequent time value token. This is
//  consistent with the behaviour for deferred and immediate pitches.
bool IsDeferredRest(const std::string& token);

// * RestGlyph *
class RestGlyph : public Glyph
{
public:
  RestGlyph();

  // Construct glyph from inpupt token and position in bar.
  RestGlyph(const std::string& inputToken, bool isWholeBar, int order);

  // Generate TIME special glyphs (for animation - we do want to highlight rests,
  //  but not generate MIDI events.)
  std::string TimeBefore() const;
  std::string TimeAfter() const;

  std::string ToString() const override;

  std::string CommentString() const override;

  // Use input token and state to generate output text for this glyph.
  std::string GetGlyphOutputStr() const;

  // For testing
  bool IsWholeBarRest() const { return m_isWholeBar; }

private:
  bool m_isWholeBar = false;
};
