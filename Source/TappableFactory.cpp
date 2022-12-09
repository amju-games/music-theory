// * Amjula music theory *
// (c) Copyright 2000-2017 Jason Colman

#include "TappableFactory.h"

namespace Amju
{
template <class T>
Tappable* CreateT() { return new T; }

TappableFactory::TappableFactory()
{
}
}

