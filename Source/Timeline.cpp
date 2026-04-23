#include <iostream>
#include <File.h>
#include "Timeline.h"

namespace Amju
{
bool Timeline::Load(File* f)
{
  // Load a timeline.
  // Have a name for timelines, it could help with debugging.
  std::string timelineName;
  if (!f->GetDataLine(&timelineName))
  {
    f->ReportError("Expected timeline name.");
    return false;
  }

  // Accumulated execution time for each event.
  // We can specify execution times as absolute times, or as offsets
  //  from the accumulated time.
  float accTime = 0;

  while (true) // "end" in file breaks out of loop
  {
    // Get event type string, use that to create event object.
    std::string eventType;
    if (!f->GetDataLine(&eventType))
    {
      f->ReportError(
        "Expected event type (or 'end') for timeline: " + 
        timelineName);
      return false;
    }

    if (eventType == "end")
      break;

    auto event = TheTimelineEventFactory::Instance()->Create(eventType);

    if (!event)
    {
      f->ReportError(
        "Could not create event type: " + 
        eventType + 
        " for timeline: " + 
        timelineName);
      return false;
    }

    event->SetTimelineName(timelineName);

    // Load time (seconds from now) when the event should execute.
    // We want to set absolute times, and also relative to the time
    //  accumulated so far. To do this, negative time means absolute;
    //  positive time means added to time so far.
    float secondsFromNow = 0;
    if (!f->GetFloat(&secondsFromNow))
    {
      f->ReportError(
        "Expected event time (seconds from now) for event: " + 
        eventType + 
        " for timeline: " +
        timelineName);
      return false;
    }

    // Event can load extra data it needs.
    if (!event->Load(f))
    {
      f->ReportError(
        "Failed to load event type: " + 
        eventType + 
        " for timeline: " + 
        timelineName);
      return false;
    }

    // Set the event execution time from now.
    // Negative time means absolute;
    //  positive time means added to time so far.
    if (secondsFromNow >= 0)
    {
      secondsFromNow += accTime;
    }
    else
    {
      secondsFromNow = std::abs(secondsFromNow);
    }
    accTime = secondsFromNow;

std::cout << "Acc time: " << accTime << "\n";
std::cout << "Time for event " << eventType << " is " << secondsFromNow << "\n";

    event->m_time = SecondsFromNow(secondsFromNow);

    // Add the event to the message queue.
    TheMessageQueue::Instance()->Add(event);
  }
  return true;
}
}

