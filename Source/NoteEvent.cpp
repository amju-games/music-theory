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
}

