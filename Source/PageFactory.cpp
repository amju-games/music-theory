// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "PageFactory.h"
#include "PageMusicalTerm.h"

namespace Amju
{
template <class T>
Page* CreateT() { return new T; }

PageFactory::PageFactory()
{
  Add(PageMusicalTerm::NAME, &CreateT<PageMusicalTerm>);
}
}

