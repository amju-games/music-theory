// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <EventPoller.h>
#include <Timer.h>
#include "GSPlayNotes.h"

namespace Amju
{
void Queue(Event* e)
{
  TheEventPoller::Instance()->GetImpl()->QueueEvent(e);
}

MouseButtonEvent* MouseEvent(bool down, float x, float y)
{
  auto mbe = new MouseButtonEvent;
  mbe->button = AMJU_BUTTON_MOUSE_LEFT;
  mbe->isDown = down;
  mbe->x = x;
  mbe->y = y;
  return mbe;
}

void SendMouseDownEvents()
{
  Queue(MouseEvent(true, 0, -0.5f));
  Queue(MouseEvent(true, 0.4f, -0.5f));
//  Queue(MouseEvent(true, -0.2f, -0.5f));
}

void SendMouseUpEvents()
{
  Queue(MouseEvent(false, 0, -0.5f));
  Queue(MouseEvent(false, 0.4f, -0.5f));
//  Queue(MouseEvent(false, -0.2f, -0.5f));
}


GSPlayNotes::GSPlayNotes()
{
  m_guiFilename = "Gui/gs_play_notes.txt";
}

void GSPlayNotes::Update()
{
  GSBase::Update();
}

void GSPlayNotes::OnActive()
{
  GSBase::OnActive();

  // Simulate mouse button and cursor (mouse move) events,
  //  to get polyphony working

  TheMessageQueue::Instance()->Add(new FuncMsg(
    SendMouseDownEvents,
    SecondsFromNow(1.f)));

  TheMessageQueue::Instance()->Add(new FuncMsg(
    SendMouseUpEvents,
    SecondsFromNow(4.f)));
}
}

