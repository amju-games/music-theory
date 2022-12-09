// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Factory.h>
#include <Singleton.h>
#include "Page.h"

namespace Amju
{
// * PageFactory *
// Create Pages, from strings which most likely come from a Topic definition.
class PageFactory : public Factory<Page>
{
public:
  PageFactory(); // populate with all Page types
};

using ThePageFactory = Singleton<PageFactory>;
}

