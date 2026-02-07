#include "Chord.h"
#include "Consts.h"
#include "NoteGlyph.h"
#include "Quad.h"
#include "Stem.h"

Quad Stem::MakeQuad() const
{
  // Our quad here is axis aligned so not too many coords to calc.
  // Quad(float xmin, float ymin, float xmax, float ymax);

  // If a single note, min and max stave are the same value, so the
  //  span is zero.
  // We're ignoring the y-coord calculated in CalcY, so we run the 
  //  same code for single notes and chords.
  // 
  // For a chord, min and max stave span the range of notes needing a stem.
  // But they are in 'stave line' coords, not actual y-coords.
  // A stave line of zero is the bottom line (e4 in treb clef, a3 in bass).
  // The next line up is stave line 2 -- gaps count (as it's integer-based).
  // So we divide by 2 to go from stave line to distances.
  // STAVE_LINE_GAP is the scale factor to go from 'half stave line' 
  //  space to actual coords.

  // This is the same calc as in CalcY
  float ymin = m_minStave * STAVE_LINE_GAP * .5f;
  float ymax = m_maxStave * STAVE_LINE_GAP * .5f;
  
  // Span for chords
  float h = ymax - ymin;

  // Add on height of stem above or below note(s)
  h += m_length * STAVE_LINE_GAP;

  float yOff = y - DEFAULT_HEIGHT + 0.475f; // Somewhere there is a fudge factor of 0.5, sigh.

  if (m_direction == Direction::UP)
  {
    //   |
    //  0
    float xOff = STEM_UP_X_OFFSET; 
    return Quad(
      (x + xOff) * GetScaleX(), 
      (ymin + yOff) * GetScaleY(), 
      (x + STEM_W + xOff) * GetScaleX(), 
      (ymin + h + yOff) * GetScaleY()); 
  }
  else
  {
    //  0
    // |
    float xOff = STEM_DOWN_X_OFFSET; 
    return Quad(
      (x + xOff) * GetScaleX(), 
      (ymax - h + yOff) * GetScaleY(), 
      (x + STEM_W + xOff) * GetScaleX(), 
      (ymax + yOff) * GetScaleY()); 
  }
}

void Stem::SetMinMaxStaveLines(int minStave, int maxStave)
{
  // Set min and max notes in a chord.
  // This length spans the notes in the chord. We then add on to this
  //  either the standard stem length, or a different distance if
  //  the chord is beamed.
  m_minStave = minStave;
  m_maxStave = maxStave;
}

std::string Stem::ToString() const
{
  std::string res;
  switch (m_lengthType)
  {
  case LengthType::NONE:
    break;

  case LengthType::STANDARD:
  case LengthType::VARIABLE:
    res += MakeQuad().ToString() + LineEnd();
    break;
  }
  
  return res; 
}

std::string Stem::CommentString() const
{
  // Don't comment if no stem is going to be drawn
  if (m_lengthType == LengthType::NONE) return "";

  std::string res = "// Stem: ";

  res += std::string("Direction: ") + 
    (m_direction == Direction::NONE ? "none" : 
      (m_direction == Direction::UP ? "up" : "down"));

  res += std::string(" LengthType: ") + 
    (m_lengthType == LengthType::NONE ? "none" : 
      (m_lengthType == LengthType::STANDARD ? "standard" : 
        (m_lengthType == LengthType::VARIABLE ? "variable" : "?")));

  if (m_isChord) res += " (CHORD)";

  return res; 
}

