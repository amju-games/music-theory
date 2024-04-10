// * Amjula music theory *
// (c) Copyright 2000-2024 Juliet Colman

#include "TappableFactory.h"

namespace Amju
{
template <class T>
Tappable* CreateT() { return new T; }

TappableFactory::TappableFactory()
{
}
}

