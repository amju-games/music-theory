#include "NoteAndChordBase.h"

void NoteAndChordBase::SetStemDirection(StemDir dir)
{
  m_stem.SetDirection(dir);
  m_stem.SetLengthType(Stem::LengthType::VARIABLE);
  m_stem.SetNoTail();
}


