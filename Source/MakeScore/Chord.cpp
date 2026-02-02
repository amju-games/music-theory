#include "Chord.h"

bool IsChordStart(const std::string& s)
{
  return s == "(";
}

bool IsChordEnd(const std::string& s)
{
  return s == ")";
}

std::string ChordGlyph::ToString() const 
{
  std::string res;
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    noteGlyph->x = this->x;
    res += noteGlyph->ToString();
  }
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

void ChordGlyph::SetTimeVal(float timeVal)
{
  Glyph::SetTimeVal(timeVal);
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    noteGlyph->SetTimeVal(timeVal);
  }
}

void ChordGlyph::SetStartTime(float st)
{
  Glyph::SetStartTime(st);
  for (const auto& noteGlyph : m_noteGlyphs)
  {
    noteGlyph->SetStartTime(st);
  }
}


