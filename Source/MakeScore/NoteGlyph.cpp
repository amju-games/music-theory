// * Amjula Music Theory *
// (c) Copyright Juliet Colman 2024
//
// * MakeScore *
// Sub-project for human-friendly authoring of musical notation.

#include <cassert>
#include "NoteGlyph.h"
#include "Suppress.h"
#include "Performance.h"

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

void NoteGlyph::CalcAccidental(KeySig ks)
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

  // Don't worry here about any Accidental already in force, we adjust
  //  later
  m_accidental = acc;
}

std::string NoteGlyph::GetGlyphOutputStr(std::string s) const
{
  std::string out = Glyph::GetGlyphOutputStr(s);

  if (s != "sb")
  {
    bool stemUp = (m_staveLine < 5);
    out += (stemUp ? "-up" : "-down");
  }

  return out;
}

void NoteGlyph::SetDisplayNameForBeamedNote()
{
  // E.g. "q" or "qq" -> "crotchet-up" for a beamed quaver.
  // Take dottedness into account.

  // TODO set stem to fit other notes connected by same beams
  bool dot = Contains(realGlyphName, '.');
  displayGlyphName = GetGlyphOutputStr(dot ? "c." : "c");
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

std::string NoteGlyph::GetStaccatoStr() const
{
  std::string res;
  if (m_switches & SW_STACCATO)
  {
    const float STACC_X_OFFSET = 0.05f;
    const float STACC_Y_OFFSET = 0.10f;
    float staccX = x + STACC_X_OFFSET;
    float staccY = y + STACC_Y_OFFSET;
    // Add dot, choosing above or below, avoiding overlapping a line
    bool stemUp = (m_staveLine < 5);
    if (stemUp)
    {
      // Staccato dot should go below
      staccY = y - STACC_Y_OFFSET;
      // Move down a bit more to skip over stave line if necessary
      if (m_staveLine == 2 || m_staveLine == 4)
      {
        staccY -= STACC_Y_OFFSET * 0.5f;
      }
    }
    else if (m_staveLine == 6)
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

std::string NoteGlyph::CommentStr() const
{
  auto res =  "// Note, " + pitch.ToString();
  if (!timevalToken.empty())
  {
    res += ", value: " + timevalToken;
  }
  res += LineEnd();
  return res;
}

std::string NoteGlyph::ToString() const
{
  // If we haven't yet created the output text, do it now
  if (displayGlyphName.empty())
  {
    // Argh, cast away constness
    const_cast<std::string&>(displayGlyphName) = GetGlyphOutputStr(realGlyphName);
  }

  std::string res;

  res += CommentStr();

  // Add special glyphs for timing before and after - this is
  //  for animation and MIDI events. 
  res += TimeBefore();

  res += displayGlyphName + ", " + Str(x) + ", " + Str(y) +
    AddScaleStringIfRequired();
  res += LineEnd();
 
  if (m_accidental != Accidental::ACCIDENTAL_NONE)
  {
    const float ACC_X_OFFSET = -0.2f;
    res += GetAccidentalStr() + ", "  + 
      Str(x + ACC_X_OFFSET) + ", " + Str(y) + 
      AddScaleStringIfRequired();
    res += LineEnd();
  }
 
  // Add ledger lines - below
  std::string ledger = "ledger";
  if (timevalToken == "sb")
  {
    ledger = "ledger-w"; // wider ledger line
  }
  for (int s = m_staveLine; s < -1; s += 2)
  {
    float ledgerY = y - (s + 2) * 0.05f;
    res += ledger + ", " + Str(x) + ", " + Str(ledgerY) +
      AddScaleStringIfRequired();
    res += LineEnd();
  }
  // Above
  for (int s = m_staveLine; s > 9; s -= 2)
  {
    float ledgerY = y - (s - 10) * 0.05f;
    res += ledger + ", " + Str(x) + ", " + Str(ledgerY) +
      AddScaleStringIfRequired();
    res += LineEnd();
  }

  // Add staccato dot
  res += GetStaccatoStr(); // staccato dot or empty str

  res += TimeAfter();

  return res;
}

std::string NoteGlyph::TimeBefore() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    float start = startTime;
    if (start == 0)
    {
      start = 0.0001f; // so first glyph is not highlighted until anim starts
    }
    float t = timeval + startTime;
    if (m_switches & SW_STACCATO)
    {
      t = timeval * 0.5f + startTime; // halve length of note
    }
    if ((GetSuppressFlags() & META_TIME) == 0)
    {
      res += "TIME, " + Str(start) + ", " + Str(t) + LineEnd();
    }
    // Output note meta data. This has escalated to pitch, volume, start
    //  time, and position on the stave. Event types have expanded to include
    //  rest on/off and should also include ties. This meta data gives
    //  the client code some understanding of the musical content, rather
    //  than just rendering primitives.
    if (   !m_tieRight 
        && (GetSuppressFlags() & META_NOTE) == 0)
    {
      // NB If we suppress times, we won't know when to play the note --
      //  note meta data should include start time. It does, no??
      // Output MIDI note event, unless on RHS of a tie
      res += "NOTE_ON, " + 
        Str(pitch.m_midi) + ", " + 
        Str(start) + ", " + 
        Str(volume) + ", " +
        Str(x) + ", " + Str(y) + 
        LineEnd();
    }
  }
  return res;
}

std::string NoteGlyph::TimeAfter() const
{
  std::string res;

  bool yesTime = (timeval > 0);
  if (yesTime)
  {
    if (   !IsRest(realGlyphName) 
        && !m_tieLeft
        && (GetSuppressFlags() & META_NOTE) == 0)
    {
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
      res += "NOTE_OFF, " + Str(pitch.m_midi) + ", " + Str(t) + LineEnd();
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
