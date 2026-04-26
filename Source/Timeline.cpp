#include <iostream>
#include <File.h>
#include <StringUtils.h>
#include "Timeline.h"
#include "TimelineEventPlayWav.h"

namespace Amju
{
TimelineEventFactory::TimelineEventFactory()
{
  // Add game-agnostic event types here
  Add(TimelineEventPlayWav::NAME,
    []()->TimelineEvent* {return new TimelineEventPlayWav; });
}

RCPtr<TimelineEvent> Timeline::CreateTimelineEvent(const std::string& eventType)
{
  // Override to set data in event
  return TheTimelineEventFactory::Instance()->Create(eventType);
}

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

    auto event = CreateTimelineEvent(eventType);

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
    std::string timeStr;
    if (!f->GetDataLine(&timeStr))
    {
      f->ReportError(
        "Expected event time (seconds from now) for event: " + 
        eventType + 
        " for timeline: " +
        timelineName);
      return false;
    }

    // If the time string contains anything like "absolute", set the flag.
    event->m_isAbsolute = StringContains(timeStr, "abs");
    float secondsFromNow = ToFloat(timeStr); // ignoring anything past the number
    event->m_time = secondsFromNow;

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

    m_events.push_back(event);
  }

  // Start timeline now or later (triggered by code or another timeline)?
  std::string nowOrLater;
  if (!f->GetDataLine(&nowOrLater))
  {
    f->ReportError("Expected 'now' or 'later' timeline string");
    return false;
  }
  if (nowOrLater == "now")
  {
    m_startNow = true;
  }
  else if (nowOrLater != "later")
  {
    f->ReportError("Expected 'now' or 'later' timeline string");
    return false;
  }

  return true;
}

void Timeline::Start()
{
  // Accumulated execution time for each event.
  // We can specify execution times as absolute times, or as offsets
  //  from the accumulated time.
  float accTime = 0;

  for (const auto& event : m_events)
  {
    // Set the event execution time from now.
    // Time can be followed by ", absolute" to set an absolute time,
    //  else the time is added on to the accumulated timeline time so far.
    if (!event->m_isAbsolute)
    {
      event->m_time += accTime;
    }
    accTime = event->m_time;

std::cout << "Acc time: " << accTime << "\n";
std::cout << "Time for event is " << event->m_time << "\n";

    // Convert 'time from now' into actual elapsed game seconds
    event->m_time = SecondsFromNow(event->m_time);

    event->OnAddToMessageQueue();

    // Add the event to the message queue.
    TheMessageQueue::Instance()->Add(event.GetPtr());
  }

  // Don't clear m_events: we can re-trigger the timeline again.
}
}

