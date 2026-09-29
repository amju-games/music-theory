#include <AmjuAssert.h>
#include "NoteEvent.h"

namespace Amju
{
std::string ToString(NoteEventType net)
{
  switch (net)
  {
  case NoteEventType::NOTE_ON: return "NOTE_ON";
  case NoteEventType::NOTE_OFF: return "NOTE_OFF";
  case NoteEventType::REST_ON: return "REST_ON";
  case NoteEventType::REST_OFF: return "REST_OFF";
  // No default, get compiler warning if we miss one
  }
  return "Fix NoteEvent::ToString";
}

std::string NoteEvent::ToString() const
{
  return 
    "ID: " + std::to_string(m_id) + " " +
    Amju::ToString(m_type) + 
    ": time: " + std::to_string(m_time) + 
    " pitch: " + std::to_string(m_note) + 
    " vol: " + std::to_string(m_volume);
}

int FindNoteOnEventForNoteOffEvent(
  const NoteEvents& events, int noteOffEventId)
{
  int i = noteOffEventId;

  Assert(i < static_cast<int>(events.size()));
  if (!events[i].IsNoteOffEvent()) return NOT_A_NOTE_OFF_EVENT;

  const int pitch = events[i].m_note;
  while (i > 0) 
  {
    --i;
    Assert(i >= 0 && i < static_cast<int>(events.size()));
    const auto& n = events[i];
    if (n.m_note == pitch && n.IsNoteOnEvent())
    {
      return i;
    }
  }
  return NOTE_ON_EVENT_NOT_FOUND;
}

int FindNoteEventForTime(const NoteEvents& noteEvents, float normalisedTime)
{
  Assert(normalisedTime >= 0.f);
  Assert(normalisedTime <= 1.f);

  if (noteEvents.empty()) return 0;

  // Binary search to find the first event with m_time > normalisedTime 
  auto it = std::upper_bound(
    noteEvents.begin(),
    noteEvents.end(),
    normalisedTime,
    [](float value, const NoteEvent& event) {
      return value < event.m_time;
    });  
 
  if (it == noteEvents.end()) 
  {
    return noteEvents.back().GetId();
  }

  return it->GetId();
}
}

