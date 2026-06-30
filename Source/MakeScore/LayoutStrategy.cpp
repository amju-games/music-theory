#include "Consts.h"
#include "LayoutStrategy.h"
#include "NoteAndChordBase.h"

float LayoutStrategy::GetTotalWidth(Glyph& g)
{
  if (auto n = dynamic_cast<NoteAndChordBase*>(&g))
  {
    // TODO does this do what we want for vertically aligned accidentals?
    int accs = n->GetNumAccidentals(); 

    // TODO get chord width
    // TODO get semibreve width
    return static_cast<float>(accs + 1) * NOTE_HEAD_WIDTH;
  }

  return NOTE_HEAD_WIDTH; // TODO
}

