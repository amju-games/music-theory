// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <SoundManager.h>
#include <Timer.h>
#include <Teapot.h>
#include "GSPlayNotes.h"
#include "PlayMidi.h"

namespace Amju
{
GSPlayNotes::GSPlayNotes()
{
  m_guiFilename = "Gui/gs_play_notes.txt";
}

void GSPlayNotes::Update()
{
  GSBase::Update();
  return;
  
  static float t = 0;
  static bool isOn = false;
  float dt = TheTimer::Instance()->GetDt();
  t += dt;
  const float NOTE_TIME = 0.2f;
  static int note = 0;
  static int oldNote = note;
  static float noteTime = 0;
  noteTime += dt * 0.25f;

  bool yesSustain = true;

  if (t > NOTE_TIME)
  {
    if (yesSustain)
    {
      PlayMidi(oldNote, 0);
    }
    else
    {
      PlayMidi(note, 0);
    }
    t = 0;
    isOn = false;
  }
  else if (t > 0 && !isOn)
  {
    oldNote = note;
    note = (int)(30 * sin(noteTime));

    PlayMidi(note, 127);

    isOn = true;
  }
}
}

