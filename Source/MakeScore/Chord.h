#pragma once

#include <vector>
#include "Glyph.h"
#include "NoteAndChordBase.h"
#include "NoteGlyph.h"
#include "Pitch.h"
#include "Stem.h"

// Chord type, populated when we parse the input tokens. 
// It's just a vec of (pitch, duration) pairs. 
// Duration of each note in the chord is the last prevailing duration
//  string, so it can change within chord markers.
using Chord = std::vector<std::pair<Pitch, std::string>>;

// For parsing MakeScore input
bool IsChordStart(const std::string&);
bool IsChordEnd(const std::string&);

// * ChordGlyph *
// This is a type of 'vertical' - maybe we should have a Vertical type.
class ChordGlyph : public NoteAndChordBase
{
public:
  ChordGlyph(const Chord& ch) : m_chordInfo(ch) {}

  std::string ToString() const override;
  std::string CommentString() const override;

  void NormaliseTimes(float scale) override;

  void SetStem();

  void AddNoteGlyph(std::unique_ptr<NoteGlyph>&& noteGlyph);

  // True if each note in the chord is beamable, i.e. < a crotchet
  bool IsBeamable() const override;
  
  int GetBeamLevel() const override;

  StemDir DecideStemDirection() override;

  int GetStaveLineForBeam(StemDir dir) override;

protected:
  std::pair<int, int> GetMinMaxStaveLines() const;

protected:
  std::vector<std::unique_ptr<NoteGlyph>> m_noteGlyphs; 

  // Do we need this? For comment, at least..?
  // Or, we use this to create the NoteGlyphs.
  Chord m_chordInfo;

  Stem m_stem;
};

