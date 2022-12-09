// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "PageFactory.h"
#include "PageIntervalsMultiChoice.h"
#include "PageMusicalTerm.h"
#include "PagePlayNotes.h"

namespace Amju
{
template <class T>
Page* CreateT() { return new T; }

PageFactory::PageFactory()
{
  Add(PageIntervalsMultiChoice::NAME, &CreateT<PageIntervalsMultiChoice>);
  Add(PageMusicalTerm::NAME, &CreateT<PageMusicalTerm>);
  Add(PagePlayNotes::NAME, &CreateT<PagePlayNotes>);
}
}

