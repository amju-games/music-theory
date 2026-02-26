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

  // *!*!* Note use of QUAD_Y_OFFSET *!*!* 
  // Quad y-coords need to add this offset to line up with glyphs
  // See definition of stave lines etc in compound_glyphs
  float yOff = y + QUAD_Y_OFFSET; 

  if (m_direction == StemDir::UP)
  {
    //   |  <-- stem
    //  0   <-- note head

    float xOff = STEM_UP_X_OFFSET; 
    return Quad(
      (x + xOff) * GetScaleX(), 
      (ymin + yOff) * GetScaleY(), 
      (x + STEM_W + xOff) * GetScaleX(), 
      (ymin + h + yOff) * GetScaleY()); 
  }
  else
  {
    //  0  <-- note head
    // |   <-- stem

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

void Stem::SetTailFromTimeType(TimeType tt)
{
  if (m_beamGroup)
  {
    m_tail = Tail::TAIL_NONE;
    return;
  }

  switch (tt)
  {
  case TimeType::QQQ:
  case TimeType::DOTTED_QQQ:
    m_tail = Tail::TAIL_QQQ;
    break;
 
  case TimeType::SEMIQUAVER:
  case TimeType::DOTTED_SEMIQUAVER:
    m_tail = Tail::TAIL_QQ;
    break;
 
  case TimeType::QUAVER:
  case TimeType::DOTTED_QUAVER:
    m_tail = Tail::TAIL_Q;
    break;

  default:
    m_tail = Tail::TAIL_NONE;
  }
}

std::string Stem::ToString() const
{
  if (m_lengthType == LengthType::NONE) return "";

  std::string res;

  res += MakeQuad().ToString() + LineEnd();
 
  res += TailString() + LineEnd();
 
  return res; 
}

std::string Stem::CommentString() const
{
  // Don't comment if no stem is going to be drawn
  if (m_lengthType == LengthType::NONE) return "";

  std::string res = "// Stem: ";

  res += std::string("Direction: ") + 
    (m_direction == StemDir::NONE ? "none" : 
      (m_direction == StemDir::UP ? "up" : "down"));

  res += std::string(" LengthType: ") + 
    (m_lengthType == LengthType::NONE ? "none" : 
      (m_lengthType == LengthType::STANDARD ? "standard" : 
        (m_lengthType == LengthType::VARIABLE ? "variable" : "?")));

  if (m_isChord) res += " (CHORD)";

  return res; 
}

std::string Stem::TailUpString() const
{
  // Urgh, stem goes up, tail goes down, sigh
  std::string tail = 
    (m_tail == Tail::TAIL_QQQ ? "tail-down-3" : 
    (m_tail == Tail::TAIL_QQ ? "tail-down-2" : 
      "tail-down-1"));

  const float TAIL_STEM_UP_X_OFFSET = 0.06f;
  float xOff = TAIL_STEM_UP_X_OFFSET;
  float yOff = (m_maxStave * .5f + m_length) * STAVE_LINE_GAP;
  return tail + ", " + Str(x + xOff) + ", " + Str(y + yOff) + 
    AddScaleStringIfRequired();
}

std::string Stem::TailDownString() const
{
  // Stem down, tail up, yuck
  std::string tail = 
    (m_tail == Tail::TAIL_QQQ ? "tail-up-3" : 
    (m_tail == Tail::TAIL_QQ ? "tail-up-2" : 
      "tail-up-1"));

  // x-coord is correct, calc y 
  float yOff = (m_minStave * .5f - m_length) * STAVE_LINE_GAP;
  return tail + ", " + Str(x) + ", " + Str(y + yOff) + AddScaleStringIfRequired();
}

std::string Stem::TailString() const
{
  if (m_tail == Tail::TAIL_NONE) return "";

  if (m_beamGroup) return "";

  std::string res;
  if (m_direction == StemDir::UP) res += TailUpString();
  else if (m_direction == StemDir::DOWN) res += TailDownString();  
  return res;
}

