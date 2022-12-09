// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiLineDrawing;

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
  GuiLineDrawing* m_drawing = nullptr;
  GuiElement* m_pencil = nullptr;
};

typedef Singleton<GSUserDraw> TheGSUserDraw;
}

