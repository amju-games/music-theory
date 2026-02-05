#include <algorithm>
#include <iostream>
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

static void CalcOverlapOffset(
  std::unique_ptr<NoteGlyph>& noteGlyph, int& lastStaveLine, int& i,
  Stem::Direction stemDir)
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
  if (stemDir == Stem::Direction::DOWN) overlap = -overlap;

  noteGlyph->SetOverlapOffset(overlap);
}

std::string ChordGlyph::ToString() const 
{
  const bool yesComment = (GetSuppressFlags() & META_COMMENT) == 0;

  // TODO Check this can't happen!
  assert(!m_noteGlyphs.empty());

  int lastStaveLine = m_noteGlyphs.front()->GetStaveLine();

  std::string res;
  int i = 0; // note counter, which resets when no overlap

  for (const auto& noteGlyph : m_noteGlyphs)
  {
    CalcOverlapOffset(const_cast<std::unique_ptr<NoteGlyph>&>(noteGlyph), 
      lastStaveLine, i, m_stem.GetDirection());

    // Set x coord of note to x coord of this chord (and it may be offset
    //  to avoid overlaps.)
    noteGlyph->x = this->x;
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
  order = noteGlyph->order;
  m_noteGlyphs.emplace_back(std::move(noteGlyph));
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
  const auto [minIt, maxIt] = std::minmax_element(
    m_noteGlyphs.begin(), m_noteGlyphs.end(), 
    [](const auto& n1, const auto& n2) 
    { 
      return n1->GetStaveLine() < n2->GetStaveLine(); 
    });

  // Decide direction
  int minStave = (*minIt)->GetStaveLine();
  int maxStave = (*maxIt)->GetStaveLine();
std::cout << "// MinStave: " << minStave << " MaxStave: " << maxStave << "\n";
  auto dir = Stem::Direction::DOWN;
  if (minStave > 4) { } // easy, all notes >= middle line
  else if (maxStave < 5) { dir = Stem::Direction::UP; }
  // Pick larger distance from mid line
  else if (maxStave - 5 > 4 - minStave) { dir = Stem::Direction::UP; }

  m_stem.SetDirection(dir);
  m_stem.SetLengthType(Stem::LengthType::VARIABLE);
  m_stem.SetMinMaxStaveLines(minStave, maxStave);

  // Is this chord beamed? How do we know if a note or chord is
  //  beamed??
  // Something like this, so stem can calc the distance to the beam.
  // NB if beamed, we ignore the direction we decided on - it has to be
  //  in the direction of the beam!
/*
  if (IsBeamed())
  {
    m_stem.SetBeam(GetBeam());
  }
*/

  m_stem.SetIsChord();
}

