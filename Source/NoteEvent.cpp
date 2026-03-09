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
}
}

