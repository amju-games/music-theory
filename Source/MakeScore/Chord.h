#pragma once

#include <vector>
#include "Glyph.h"
#include "NoteGlyph.h"
#include "Pitch.h"
#include "Stem.h"

// Chord type, populated when we parse the input tokens. 
// It's just a vec of (pitch, duration) pairs. 
// Duration of each note in the chord is the last prevailing duration
//  string, so it can change within chord markers.
// TODO Duration is currently a string, should be an enum or type.
using Chord = std::vector<std::pair<Pitch, std::string>>;

// For parsing MakeScore input
bool IsChordStart(const std::string&);
bool IsChordEnd(const std::string&);

// * ChordGlyph *
// This is a type of 'vertical' - maybe we should have a Vertical type.
// TODO Derive from NoteGlyph? 
class ChordGlyph : public Glyph
{
public:
  ChordGlyph(const Chord& ch) : m_chordInfo(ch) {}

  std::string ToString() const override;
  std::string CommentString() const override;

  void SetStem(const Stem& stem) { m_stem = stem; }

  // Override these Glyph functions to set times on child Notes.
  void SetTimeVal(float timeVal) override; 
  void SetStartTime(float st) override;
 
  void AddNoteGlyph(std::unique_ptr<NoteGlyph>&& noteGlyph);

  // Overridden to set stem
  void SetScale(float s) override;
  void SetScale(float sx, float sy) override;
  void SetPos(float x_, float y_) override;

protected:
  GlyphVec m_noteGlyphs; 

  // Do we need this? For comment, at least..?
  // Or, we use this to create the NoteGlyphs.
  Chord m_chordInfo;

  Stem m_stem;
};

