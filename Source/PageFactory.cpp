// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "PageFactory.h"
#include "PageIntervalsMultiChoice.h"
#include "PageMusicalTerm.h"
#include "PageMusicalSymbol.h"
#include "PagePlayNotes.h"

namespace Amju
{
template <class T>
Page* CreateT() { return new T; }

Page* CreatePageNoteLengthNames()
{
  PageMusicalSymbol* page = new PageMusicalSymbol;
  page->SetDictionaryName("Course/note_lengths.txt.dictionary");
  return page;
}

PageFactory::PageFactory()
{
  Add(PageIntervalsMultiChoice::NAME, &CreateT<PageIntervalsMultiChoice>);
  Add(PageMusicalSymbol::NAME, &CreateT<PageMusicalSymbol>);
  Add(PageMusicalTerm::NAME, &CreateT<PageMusicalTerm>);
  Add(PagePlayNotes::NAME, &CreateT<PagePlayNotes>);
  Add("page-note-length-names", CreatePageNoteLengthNames);
}
}

