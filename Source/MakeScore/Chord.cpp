#include <algorithm>
#include <iostream>
#include <set>
#include "Chord.h"
#include "Suppress.h"

bool IsChordStart(const std::string& s)
{
  return s == "(";
}

bool IsChordEnd(const std::string& s)
{
  return s == ")";
}

// Calc x-offset for a note in a chord.
// We offset to avoid notes on adjacent stave lines from overlapping.
// Returns offset appied to note, which can be -1, 0, or 1.
static int CalcOverlapOffset(
  std::unique_ptr<NoteGlyph>& noteGlyph, int& lastStaveLine, int& i,
  StemDir stemDir)
{
  // Calc offset to avoid overlap
  int overlap = 0;
  int thisStaveLine = noteGlyph->GetStaveLine();

  if (   thisStaveLine == lastStaveLine + 1
      || thisStaveLine == lastStaveLine - 1)
  {
    overlap = 1;
  }
  else
  {
    // No overlap: reset the overlap counter
    lastStaveLine = thisStaveLine;
    i = 0;
  }
  // Count notes, so we stagger multiple contiguous overlapping notes.
  if (i++ == 2) lastStaveLine = thisStaveLine;

  // Flip side of stem we offset to depending on its direction
  if (stemDir == StemDir::DOWN) overlap = -overlap;

  noteGlyph->SetOverlapOffset(overlap);

  return overlap;
}

std::string ChordGlyph::ToString() const 
{
  const bool yesComment = (GetSuppressFlags() & META_COMMENT) == 0;

  // TODO Check this can't happen!
  assert(!m_noteGlyphs.empty());

  int lastStaveLine = m_noteGlyphs.front()->GetStaveLine();

  std::string res;
  int i = 0; // note counter, which resets when no overlap

  // Remember any notes (their stave line, anyway) where we offset the
  //  note to the left, because this affects the offset of any 
  //  accidental on the note. (If we offset to the right, it doesn't
  //  affect the accidental offset.)
  std::set<int> offsetLeftNotes;

  // Store stave lines which have notes with accidentals on them.
  std::set<int> accidentalStaveLines;

  // Calc offsets for overlapping notes, and gather stave lines with 
  //  note offsets and accidentals. 
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    int noteOffset = CalcOverlapOffset(
      const_cast<std::unique_ptr<NoteGlyph>&>(noteGlyph), 
      lastStaveLine, i, m_stem.GetDirection());

    if (noteOffset == -1)
    {
      offsetLeftNotes.insert(noteGlyph->GetStaveLine());
    }

    if (noteGlyph->GetAccidental() != Accidental::ACCIDENTAL_NONE)
    {
      accidentalStaveLines.insert(noteGlyph->GetStaveLine());
    }
  }

  // Second pass: now we can calc offsets for accidentals and output
  //  the note.
  int accidentalOffset = 0;
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    // If we are far enough away from other notes' stave lines, we
    //  can reset the offset applied
    const int staveLine = noteGlyph->GetStaveLine();
    if (offsetLeftNotes.contains(staveLine))
    {
      accidentalOffset++;
    }
    if (accidentalStaveLines.contains(staveLine))
    {
      noteGlyph->SetAccidentalOverlapOffset(accidentalOffset);
      accidentalOffset++;
    }

    // Set x coord of note to x coord of this chord (and it may be offset
    //  to avoid overlaps.)
    noteGlyph->x = this->x;
    noteGlyph->y += this->y; // add vertical offset, for multiple staves
    res += noteGlyph->ToString();
  }

  // Output stem
  const_cast<Stem&>(m_stem).SetScale(GetScaleX(), GetScaleY());
  const_cast<Stem&>(m_stem).SetPos(GetX(), GetY());

  if (yesComment) res += m_stem.CommentString() + LineEnd();
  res += m_stem.ToString();

  return res;
}

std::string ChordGlyph::CommentString() const 
{
  std::string res = "// Chord: ( ";
  for (const auto& [pitch, duration] : m_chordInfo)
  {
    res += pitch.ToString() + " " + duration + "  ";
  }
  res += " )";
  return res;
}

void ChordGlyph::AddNoteGlyph(std::unique_ptr<NoteGlyph>&& noteGlyph)
{
  //order = noteGlyph->order;
  noteGlyph->SetIsPartOfChord(true);
  m_noteGlyphs.emplace_back(std::move(noteGlyph));
}

std::pair<int, int> ChordGlyph::GetMinMaxStaveLines() const
{
  const auto [minIt, maxIt] = std::minmax_element(
    m_noteGlyphs.begin(), m_noteGlyphs.end(), 
    [](const auto& n1, const auto& n2) 
    { 
      return n1->GetStaveLine() < n2->GetStaveLine(); 
    });

  int minStave = (*minIt)->GetStaveLine();
  int maxStave = (*maxIt)->GetStaveLine();

  return { minStave, maxStave };
}

void ChordGlyph::SetStem()
{
  bool shouldHaveStem = false;
  // Check for needing a stem at all
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    if (noteGlyph->ShouldHaveStem()) 
    {
      shouldHaveStem = true;
    }
  }

  if (!shouldHaveStem)
  {
    return;
  }

  // Get min and max stave lines
  const auto [minStave, maxStave] = GetMinMaxStaveLines();
  m_stem.SetMinMaxStaveLines(minStave, maxStave);

  // All chords have variable length stems
  m_stem.SetLengthType(Stem::LengthType::VARIABLE);

  // Is this chord beamed? How do we know if a note or chord is
  //  beamed??
  // Something like this, so stem can calc the distance to the beam.
  // NB if beamed, we ignore the direction we decided on - it has to be
  //  in the direction of the beam!

  auto beamGroup = GetBeamGroup();
  if (beamGroup)
  {
    // This chord is beamed: - stem dir is set by beam
    m_stem.SetBeamGroup(beamGroup);
    m_stem.SetDirection(beamGroup->GetStemDirection());
  }
  else
  {
    DecideStemDirection();
 
    // Set tail -- no tail if beamed. 
    m_stem.SetTailFromTimeType(GetTimes().GetTimeType());
  }

  m_stem.SetIsChord();
}

StemDir ChordGlyph::DecideStemDirection()
{
  const auto [minStave, maxStave] = GetMinMaxStaveLines();

  auto dir = StemDir::DOWN;
  if (minStave > 4) { } // easy, all notes >= middle line
  else if (maxStave < 5) { dir = StemDir::UP; }
  // Pick larger distance from mid line
  else if (maxStave - 5 > 4 - minStave) { dir = StemDir::UP; }
  m_stem.SetDirection(dir);

  return dir;
}

void ChordGlyph::NormaliseTimes(float scale)
{
  GetTimes().Normalise(scale);
  // Normalise note times too
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    noteGlyph->GetTimes().Normalise(scale);
  }
}

bool ChordGlyph::IsBeamable() const
{
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    if (!noteGlyph->IsBeamable()) return false;
  }
  return true; 
}

