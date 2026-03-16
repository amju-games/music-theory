// * Amjula music theory *
// (c) Copyright 2000-2024 Juliet Colman

#pragma once

#include <Factory.h>
#include "Tappable.h"

namespace Amju
{
class TappableFactory : public Factory<Tappable>
{
public:
  TappableFactory();
};

using TheTappableFactory = Singleton<TappableFactory>;

}

