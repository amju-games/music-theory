// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiSpline;

// * GSUserDraw *
// User can draw on screen.
class GSUserDraw : public GSBase
{
public:
  virtual void Update() override;
  virtual void OnActive() override;

  virtual bool OnCursorEvent(const CursorEvent&) override;
  virtual bool OnMouseButtonEvent(const MouseButtonEvent&) override;
  virtual bool OnDoubleClickEvent(const DoubleClickEvent&) override;

protected:
  bool m_isDrawing = false;
  GuiSpline* m_drawing = nullptr;
  GuiElement* m_pencil = nullptr;
};

typedef Singleton<GSUserDraw> TheGSUserDraw;
}

