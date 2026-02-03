#include "Chord.h"
#include "Consts.h"
#include "NoteGlyph.h"
#include "Quad.h"
#include "Stem.h"

Quad Stem::MakeQuad() const
{
  // If a chord, span the range of notes, then add on the 
  //  length -- either standard or to reach the beam if beamed.
  // For single note, if we're here, we must be beamed I think.

  // Our quad here is axis aligned so not too many coords to calc.
  // Quad(float xmin, float ymin, float xmax, float ymax);
  float h = m_length * STAVE_LINE_GAP;
  float xOff = 0.05f; // Where do we find this number tho
  float yOff = 0.475f;
  if (m_direction == Direction::UP)
  {
    xOff += 0.2f; // note head width
    return Quad(
      (x + xOff) * GetScaleX(), 
      (y + yOff) * GetScaleY(), 
      (x + STEM_W + xOff) * GetScaleX(), 
      (y + h + yOff) * GetScaleY()); 
  }
  else
  {
    return Quad(
      (x + xOff) * GetScaleX(), 
      (y - h + yOff) * GetScaleY(), 
      (x + STEM_W + xOff) * GetScaleX(), 
      (y + yOff) * GetScaleY()); 
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
  // Oh wait, can we just use the same code, ensuring consistency?
/*
    res += std::string(m_direction == Direction::UP ? "stem-up" : "stem-down") + 
      ", " + Str(x) + ", " + Str(y) +
      AddScaleStringIfRequired() + 
      LineEnd();
    break;
*/
  case LengthType::VARIABLE:
    // Make a quad. 
    res += MakeQuad().ToString() + LineEnd();
    break;
  }
  
  return res; 
}

std::string Stem::CommentString() const
{
  std::string res = "// Stem: ";
  res += std::string("Direction: ") + 
    (m_direction == Direction::NONE ? "none" : (m_direction == Direction::UP ? "up" : "down"));
  res += std::string(" LengthType: ") + 
    (m_lengthType == LengthType::NONE ? "none" : (m_lengthType == LengthType::STANDARD ? "standard" : (m_lengthType == LengthType::VARIABLE ? "variable" : "?")));
  if (m_isChord) res += " (CHORD)";
  return res; 
}

