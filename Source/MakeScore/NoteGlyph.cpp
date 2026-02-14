// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <array>
#include <cassert>
#include <iostream>
#include "NoteGlyph.h"
#include "Suppress.h"
#include "Performance.h"

static int CalcStaveLineForStepOctaveAlterPitch(
 KeySig keySig, Clef clef, const Pitch& pitch) 
{
  if (clef == Clef::CLEF_PERCUSSION)
    return PERCUSSION_STAVE_LINE;

  // Calc stave line for step, with C at zero.
  const std::array<int, 7> LINES = {{ 5, 6, 0, 1, 2, 3, 4 }};
  int i = std::tolower(pitch.m_step[0]) - 'a';
  i = std::clamp(i, 0, 6);
  assert(i >= 0);
  assert(i < 7);
  int line = LINES[i];

  // Offset, depending on clef
  const int CLEF_OFFSET[] = 
  { 
    0, // CLEF_NONE,
    0, // CLEF_PERCUSSION,
    -2, // CLEF_TREBLE,
    10, // CLEF_BASS,
    4, // CLEF_ALTO,
    6, // CLEF_TENOR, 
  };  
  line += CLEF_OFFSET[static_cast<int>(clef)];
  // Octave shift, where octave 4 is a shift of zero
  int octaveShift = (pitch.m_octave - 4) * 7;
  line += octaveShift;
  // pitch.m_alter doesn't alter the line the note is on.

  return line;
}

static int CalcStaveLineForMidiPitch(KeySig keySig, Clef clef, const Pitch& pitch) 
{
  if (clef == Clef::CLEF_PERCUSSION)
    return PERCUSSION_STAVE_LINE;

  // Y-position for MIDI notes starting from MIDI 0, with C at y = 0.
  // y = 0 corresponds to the bottom line of the stave.
  // Choose array depending on whether the current key sig uses 
  //  sharps or flats.
  const int Y_POS[2][12] = 
  {   
    { 0, 0, 1, 1, 2, 3, 3, 4, 4, 5, 5, 6 }, // Sharp key sig
    { 0, 1, 1, 2, 2, 3, 4, 4, 5, 5, 6, 6 }  // Flat key sig 
  };  

  // Add this offset to the y position, setting y to the correct
  //  position on the stave. E.g. for treble clef, middle C should be
  //  at y = -2, i.e. two stave positions below the bottom line.
  const int CLEF_OFFSET[] = 
  { 
    0, // CLEF_NONE,
    0, // CLEF_PERCUSSION,
    -2, // CLEF_TREBLE,
    10, // CLEF_BASS,
    4, // CLEF_ALTO,
    6, // CLEF_TENOR, 
  };  

  // Choose stave position depending on key sign type (sharp/flat)
  int sharpOrFlat = (keySig >= KEYSIG_0_FLAT) ? 1 : 0;
  int staveLine = Y_POS[sharpOrFlat][pitch.m_midi % 12] + CLEF_OFFSET[static_cast<int>(clef)];
  // Use the octave to shunt note up or down
  int octave = (pitch.m_midi / 12 - 5) * 7; // so middle C is 0
  staveLine += octave;
  return staveLine;
}
 
int NoteGlyph::CalcStaveLine(KeySig keySig, Clef clef, const Pitch& pitch) 
{
  if (pitch.m_step.empty())
  {
    return CalcStaveLineForMidiPitch(keySig, clef, pitch);
  }
  else
  {
    return CalcStaveLineForStepOctaveAlterPitch(keySig, clef, pitch);
  }
}

void NoteGlyph::CalcY(KeySig keySig, Clef clef) 
{
  int staveLine = CalcStaveLine(keySig, clef, m_pitch);
  SetStaveLine(staveLine);

  // Scale stave position by unit of distance in our coordinate system.
  // Convert stave line unit into distance: Each stave line unit is
  //  half the distance between two adjacent stave lines, because the
  //  gaps are counted too. (e.g. c4 = -2, d4 = -1, e4 = 0 etc.)
  float y = static_cast<float>(staveLine) * STAVE_LINE_GAP * .5f;
  // TODO Offset y for stave > 1? Should be done in Bar.
  SetY(y);
}

void NoteGlyph::AdjustAccidental(Accidental previousAcc)
{
  // Don't repeat an accidental, e.g. two sharp notes in sequence
  if (m_accidental == previousAcc)
  {
    m_accidental = Accidental::ACCIDENTAL_NONE;
  }

  // Don't put natural sign unnecessarily
  if (m_accidental == Accidental::ACCIDENTAL_NATURAL_IN_KEY_SIG &&
      (previousAcc == Accidental::ACCIDENTAL_NONE ||
       previousAcc == Accidental::ACCIDENTAL_NATURAL))
  {
    m_accidental = Accidental::ACCIDENTAL_NONE;
  } 

  // Don't flatten a note where key sig already does so
  if (m_accidental == Accidental::ACCIDENTAL_FLAT_IN_KEY_SIG &&
      (previousAcc == Accidental::ACCIDENTAL_NONE ||
       previousAcc == Accidental::ACCIDENTAL_FLAT))
  {
    m_accidental = Accidental::ACCIDENTAL_NONE;
  } 

  // Don't sharpen a note where key sig already does so
  if (m_accidental == Accidental::ACCIDENTAL_SHARP_IN_KEY_SIG &&
      (previousAcc == Accidental::ACCIDENTAL_NONE ||
       previousAcc == Accidental::ACCIDENTAL_SHARP))
  {
    m_accidental = Accidental::ACCIDENTAL_NONE;
  } 
}

Accidental NoteGlyph::CalcAccidental(KeySig ks)
{
  Accidental acc;
  if (m_pitch.m_step.empty())
  {
    acc = CalcAccidentalFromMidi(ks, m_pitch);
  }
  else
  {
    acc = CalcAccidentalFromStepOctAlter(ks, m_pitch);
  }
  // Don't worry here about any Accidental already in force, we adjust
  //  later
  m_accidental = acc;
  return acc;
}

Accidental NoteGlyph::CalcAccidentalFromStepOctAlter(KeySig ks, Pitch pitch)
{
  int i = std::clamp(pitch.m_alter + 2, 0, 4);
  const std::array<Accidental, 5> ACCS = 
  {{
    Accidental::ACCIDENTAL_DOUBLE_FLAT,
    Accidental::ACCIDENTAL_FLAT,
    Accidental::ACCIDENTAL_NATURAL,
    Accidental::ACCIDENTAL_SHARP,
    Accidental::ACCIDENTAL_DOUBLE_SHARP,
  }};
  assert(i >= 0);
  assert(i < 5);
  auto acc = ACCS[i];

  // Now get the accidental for the key and midi pitch. If the accidental
  //  above is already set from the key sig, we don't need it.
  auto keySigAcc = CalcAccidentalFromMidi(ks, pitch);

  if (   (acc == Accidental::ACCIDENTAL_SHARP && 
          keySigAcc == Accidental::ACCIDENTAL_SHARP_IN_KEY_SIG)
      || (acc == Accidental::ACCIDENTAL_FLAT && 
          keySigAcc == Accidental::ACCIDENTAL_FLAT_IN_KEY_SIG)
      || (acc == Accidental::ACCIDENTAL_NATURAL && 
          keySigAcc == Accidental::ACCIDENTAL_NATURAL_IN_KEY_SIG))
  {
    acc = keySigAcc;
  }

  return acc;
}

Accidental NoteGlyph::CalcAccidentalFromMidi(KeySig ks, Pitch pitch)
{
  // Is pitch in ks, or do we need an accidental?
  // Get note 0..11, then look up note in the given key 
  int note = pitch.m_midi % 12;

  const auto S = Accidental::ACCIDENTAL_SHARP;
  const auto s = Accidental::ACCIDENTAL_SHARP_IN_KEY_SIG;
  const auto F = Accidental::ACCIDENTAL_FLAT;
  const auto f = Accidental::ACCIDENTAL_FLAT_IN_KEY_SIG;
  const auto N = Accidental::ACCIDENTAL_NATURAL;
  const auto _ = Accidental::ACCIDENTAL_NATURAL_IN_KEY_SIG;

  // Array of accidental to apply to a given pitch, for each key  
  const Accidental ACCS[16][12] = 
  {  
    //  Note -->
    //  C  C# D  D# E  F  F# G  G# A  A# B    // MIDI Pitch
    //  60 61 62 63 64 65 66 67 68 69 70 71   // E.g. from middle C

    {   _, S, _, S, _, _, S, _, S, _, S, _ }, // key s0 (i.e. c maj)
    {   _, S, _, S, _, N, s, _, S, _, S, _ }, // key s1 (g maj)
    {   N, s, _, S, _, N, s, _, S, _, S, _ }, // key s2 (d maj)
    {   N, s, _, S, _, N, s, N, s, _, S, _ }, // key s3
    {   N, s, N, s, _, N, s, N, s, _, S, _ }, // key s4
    {   N, s, N, s, _, N, s, N, s, N, s, _ }, // key s5
    {   N, s, N, s, s, N, s, N, s, N, s, _ }, // key s6
    {   N, s, N, s, s, N, s, N, s, N, s, s }, // key s7

    //  C  Db D  Eb E  F  Gb G  Ab A  Bb B

    {   _, F, _, F, _, _, F, _, F, _, F, _ }, // key f0 (i.e. c maj)
    {   _, F, _, F, _, _, F, _, F, _, f, N }, // key f1 (i.e. f maj)
    {   _, F, _, f, N, _, F, _, F, _, f, N }, // key f2
    {   _, F, _, f, N, _, F, _, f, N, f, N }, // key f3
    {   _, f, N, f, N, _, F, _, f, N, f, N }, // key f4
    {   _, f, N, f, N, _, f, N, f, N, f, N }, // key f5
    {   N, f, N, f, N, _, f, N, f, N, f, N }, // key f6
    {   N, f, N, f, N, N, f, N, f, N, f, N }  // key f7
  };

  Accidental acc = ACCS[ks][note];
  return acc;
}

std::string NoteGlyph::GetGlyphOutputStr() const
{
  // Decide glyph code for note head glyph
  switch (m_times.GetTimeType())
  {
  case TimeType::MINIM:
  case TimeType::DOTTED_MINIM:
    return "note-minim";

  case TimeType::SEMIBREVE:
  case TimeType::DOTTED_SEMIBREVE:
    return "semibreve";

  default:
    break;
  }

  // Any note crotchet or shorter time value gets solid note head.
  return "note-solid"; // glyph code for standard solid note head
}

std::string NoteGlyph::GetAccidentalStr() const
{
  const std::string ACC_STR[] = 
  {
    "",
    "natural",
    "natural", 
    "sharp",
    "sharp",
    "flat",
    "flat",
    "double-sharp",
    "double-flat"
  };
  return ACC_STR[static_cast<int>(m_accidental)];
}

std::string NoteGlyph::StaccatoString() const
{
  std::string res;
  if (m_switches & SW_STACCATO)
  {
    const float STACC_X_OFFSET = 0.05f;
    const float STACC_Y_OFFSET = 0.10f;
    float staccX = x + STACC_X_OFFSET;
    float staccY = y + STACC_Y_OFFSET;
    // Add dot, choosing above or below, avoiding overlapping a line
    bool stemUp = (m_stem.GetDirection() == Stem::Direction::UP);

    // If on one of these stave lines, we should move up or down to 
    //  avoid overlapping a line.
    // We DON'T need to worry about ledger lines: if this is a single
    //  note, there won't be any ledger line beyond this note; if this 
    //  note is in a chord, we will only draw the dot for the highest
    //  or lowest note, so that still holds true.
    bool onLine = (m_staveLine == 2 || m_staveLine == 4 || m_staveLine == 6);
    if (stemUp)
    {
      // Staccato dot should go below
      staccY = y - STACC_Y_OFFSET;
      // Move down a bit more to skip over stave line if necessary
      if (onLine)
      {
        staccY -= STACC_Y_OFFSET * 0.5f;
      }
    }
    else if (onLine)
    {
      // Move down a bit more to skip over stave line if necessary
      staccY += STACC_Y_OFFSET * 0.5f;
    }
   
    // Dot glyph name is just '.' 
    res = "., " + Str(staccX) + ", " + Str(staccY) + 
      AddScaleStringIfRequired();
    res += LineEnd();
  }
  return res;
}

std::string NoteGlyph::CommentString() const
{
  auto res =  "// Note, " + m_pitch.ToString();
  const auto& token = m_times.GetTimeToken();
  if (!token.empty())
  {
    res += ", duration: " + token;
  }
  res += LineEnd();
  return res;
}

std::string NoteGlyph::DotString() const
{
  std::string res;
  if (m_times.IsDotted())
  {
    // Are we on a line? If so, raise the dot to avoid overlapping the line.
    bool onLine = m_staveLine % 2 == 0; 
    // But if we're on a ledger line and offset to the right, don't raise
    //  the dot, as it will be beyond the ledger line, so no point in
    //  raising it.
    if (onLine && (m_staveLine < 0 || m_staveLine > 8) && m_overlapOffset == 1)
    {
      onLine = false;
    }
    // Sigh, actually I think the above is wrong. If we offset to the right,
    //  the ledger lines should be wide enough for offset and non-offset 
    //  notes, and so we would still want to raise the dot.
    std::string dotType = onLine ? "raised-dot" : "reg-dot";

    // Wide note?
    if (m_times.DurationIsSemibreveOrMore()) dotType = "sb-" + dotType;
  
    // If the note head is offset to the right, also offset the dot to
    //  the right.
    float xDotOffset = (m_overlapOffset == 1 ? NOTE_HEAD_WIDTH : 0.f);  

    // Output the dot.
    res += dotType + ", " + Str(x + xDotOffset) + ", " + Str(y) + 
      AddScaleStringIfRequired() + 
      LineEnd();
  }
  return res;
}

std::string NoteGlyph::AccidentalString() const
{
  std::string res;
  if (m_accidental != Accidental::ACCIDENTAL_NONE)
  {
    // Accidentals get offset to the left for single notes. For
    //  chords, we can offset more to avoid overlapping accidentals.
    // (The offset is negative as to the left of the note head.)
    float accXOffset = -(ACCIDENTAL_X_OFFSET +
      ACCIDENTAL_EXTRA_OFFSET *  
      static_cast<float>(m_accidentalOverlapOffsets));

    res += GetAccidentalStr() + ", "  + 
      Str(x + accXOffset) + ", " + Str(y) + 
      AddScaleStringIfRequired();
    res += LineEnd();
  }
  return res;
}

LedgerLineWidth NoteGlyph::DecideLedgerLineWidth() const
{
  // Decide the width of ledger line we need. 
  //  0. suppress ledger line
  //  1. single note, duration < sb
  //  2. single sb

  if (m_times.DurationIsSemibreveOrMore())
    return LedgerLineWidth::SINGLE_SB;

  return LedgerLineWidth::SINGLE_NOTE;
}

std::string NoteGlyph::LedgerLinesString(LedgerLineWidth width) const
{
  std::string res;

  // Decide the width of ledger line we need. 
  //  0. suppress ledger line
  //  1. single note, duration < sb
  //  2. single sb
  //  For notes in a chord, these are also possible:
  //  3. overlap offset notes < sb
  //  4. overlap offset sbs
  // The offset has to be decided by the chord for 3 and 4. The overlap offset
  //  could have been applied to another note, not this one.
  if (static_cast<int>(width) == 0) return res;

  static const std::array<std::string, 5> LEDGERS = 
  {{
     "** Ledger line ERROR **",
     "ledger",
     "ledger-w",
     "ledger-w", // TODO
     "ledger-w", // TODO
  }};

  std::string ledger = LEDGERS[static_cast<int>(width)];

  // Draw lines under the stave, coming up from the lowest.
  for (int s = m_staveLine; s < -1; s += 2)
  {
    // Convert from stave line to actual y-coord: this is the
    //  same calculation as in CalcY.
    float ledgerY = y - (s + 2) * 0.5f * STAVE_LINE_GAP;

    res += ledger + ", " + Str(x) + ", " + Str(ledgerY) +
      AddScaleStringIfRequired() +
      LineEnd();
  }

  // Draw lines above the stave, coming down.
  for (int s = m_staveLine; s > 9; s -= 2)
  {
    float ledgerY = y - (s - 10) * 0.5f * STAVE_LINE_GAP;

    res += ledger + ", " + Str(x) + ", " + Str(ledgerY) +
      AddScaleStringIfRequired() + 
      LineEnd();
  }

  return res;
}

std::string NoteGlyph::StemString() const
{
  std::string res;

  const bool yesComment = (GetSuppressFlags() & META_COMMENT) == 0;

  // Output stem
  const_cast<Stem&>(m_stem).SetScale(GetScaleX(), GetScaleY());
  // Urgh, we want the vertical offset for this stave, but we don't want
  //  the note head position (added in CalcY), because we would then be
  //  adding the note head position to the y-coord twice.
  // So remove the note head component of the y coord. What a mess,
  //  TODO fix this travesty. 
  float awfulHackY = y - 
    static_cast<float>(m_staveLine) * STAVE_LINE_GAP * .5f;

  const_cast<Stem&>(m_stem).SetPos(GetX(), awfulHackY);

  if (yesComment) 
  {
     res += m_stem.CommentString() + LineEnd();
  }
  res += m_stem.ToString() + LineEnd();

  return res;
}

std::string NoteGlyph::NoteHeadString() const
{
  std::string res;
  // Output note head: x position can be offset left or right in a chord.
  float xOffset = static_cast<float>(m_overlapOffset) * NOTE_HEAD_OVERLAP_OFFSET;
  
  res += GetGlyphOutputStr() + ", " + Str(x + xOffset) + ", " + Str(y) + 
    AddScaleStringIfRequired() + 
    LineEnd();
 
  return res;
}

std::string NoteGlyph::ToString() const
{
  std::string res;

  // Add special glyphs for timing before and after - this is
  //  for animation and MIDI events. 
  res += TimeBefore();

  res += NoteHeadString();

  res += StemString();

  res += DotString(); // Output duration * 1.5 dot if required

  res += AccidentalString();
 
  // For a single note, we decide the ledger line width; for notes in
  //  a chord, the chord decides.
  res += LedgerLinesString(DecideLedgerLineWidth());

  // Add staccato dot -- TODO like stems, this needs to be done by owning chord
  //  if we are part of a chord.
  res += StaccatoString(); // staccato dot or empty str

  res += TimeAfter();

  return res;
}

std::string NoteGlyph::TimeBefore() const
{
  std::string res;

  const float durationNorm = m_times.GetNormalisedDuration();
  bool yesTime = (durationNorm > 0);
  if (yesTime)
  {
    float startTimeNorm = m_times.GetNormalisedStartTime();
    startTimeNorm = std::max(MIN_START_TIME, startTimeNorm);
    
    float endTimeNorm = startTimeNorm + durationNorm;
    if (m_switches & SW_STACCATO)
    {
      endTimeNorm = durationNorm * 0.5f + startTimeNorm; // halve length of note
    }

    if ((GetSuppressFlags() & META_TIME) == 0)
    {
      res += "TIME, " + Str(startTimeNorm) + ", " + Str(endTimeNorm) + LineEnd();
    }
    // Output note meta data. This has escalated to pitch, volume, start
    //  time, and position on the stave. Event types have expanded to include
    //  rest on/off and should also include ties. This meta data gives
    //  the client code some understanding of the musical content, rather
    //  than just rendering primitives.
    if (   !m_tieRight 
        && (GetSuppressFlags() & META_NOTE) == 0)
    {
      // Output MIDI note event, unless on RHS of a tie
      res += "NOTE_ON, " + 
        Str(m_pitch.m_midi) + ", " + 
        Str(startTimeNorm) + ", " + 
        Str(m_volume) + ", " +
        Str(x) + ", " + Str(y) + 
        LineEnd();
    }
  }
  return res;
}

std::string NoteGlyph::TimeAfter() const
{
  std::string res;

  const float timeval = m_times.GetNormalisedDuration();
  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    if (   !m_tieLeft 
        && (GetSuppressFlags() & META_NOTE) == 0)
    {
      const float startTime = m_times.GetNormalisedStartTime();

      // Output MIDI note off event, unless the note is on LHS of a tie,
      //  in which case it will last longer.
      // Follow chain of ties back to start of tie, to get total length.
      // TODO  Tied notes

      // NB Reducing time of note by 10%. This is so there is a short
      //  break between note on and note off events, which will prevent
      //  simultaneous note on/off events for the same note, which can
      //  only be bad news.
      // TODO We could vary this amount depending on legato, etc.
      float timeReduction = 0.9f;
      float t = timeval * timeReduction + startTime;
      if (m_switches & SW_STACCATO)
      {
        t = timeval * 0.5f + startTime; // halve length of note
      }
      res += "NOTE_OFF, " + Str(m_pitch.m_midi) + ", " + Str(t) + LineEnd();
    }

    // Cancel time for subsequent glyphs (but postprocess to strip out
    //  unnecessary cancellations)
    if ((GetSuppressFlags() & META_TIME) == 0)
    {
      res += "TIME, -1, -1" + LineEnd();
    }
  }
  return res;
}

bool NoteGlyph::ShouldHaveStem() const
{
  return !m_times.DurationIsSemibreveOrMore();
}

void NoteGlyph::SetStem()
{
  if (!ShouldHaveStem()) 
  {
    return;
  }

  m_stem.SetDirection(GetStaveLine() < 5 ? 
    Stem::Direction::UP : Stem::Direction::DOWN);

  m_stem.SetLengthType(Stem::LengthType::STANDARD); // TODO VARIABLE if beamed

  // TODO We will calc length if beamed, and ignore direction, as we have
  //  to connect with the beam!

  m_stem.SetMinMaxStaveLines(m_staveLine, m_staveLine); // same for min and max

  m_stem.SetTailFromTimeType(GetTimes().GetTimeType());
}

