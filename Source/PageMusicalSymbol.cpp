// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "PageMusicalSymbol.h"

namespace Amju
{
const char* PageMusicalSymbol::NAME = "page-musical-symbol";

PageMusicalSymbol::PageMusicalSymbol()
{
  m_guiName = "musical_symbol";
  m_dictionaryFilename = "Course/musical_symbols.txt.dictionary";
}
}
