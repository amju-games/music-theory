// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "GSUserDraw.h"

namespace Amju
{
// * GSUserDrawSave *
// Let user save drawing to file, good for creating strokes
class GSUserDrawSave : public GSUserDraw
{
public:
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
};

typedef Singleton<GSUserDrawSave> TheGSUserDrawSave;
}

