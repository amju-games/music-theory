#pragma once

#include <Factory.h>
#include <MessageQueue.h>

// For title and other scripted animations, this is a Timeline
//  we can load in from a file. The timeline has messages, each of which
//  executes at a scripted time from when loaded.
namespace Amju
{
class File;

// * TimelineEvent *
// A message type which is used to trigger animations etc in a scripted
//  timeline (e.g. for a cut scene).
struct TimelineEvent : public Message
{
  virtual bool Load(File*) { return true; }

  // Override to do something when this event is added to queue.
  virtual void OnAddToMessageQueue() {}

  // Set owning timeline name, for debugging
  void SetTimelineName(const std::string& s) { m_timelineName = s; }

  std::string m_timelineName;
  bool m_isAbsolute = false; // time can be absolute or relative to prev event.
};

// * TimelineEventWait *
// Pause the current timeline until a resume event is triggered.
struct TimelineEventWait : public TimelineEvent
{
  static const char* NAME;
  void Execute() override {}
};

// * TimelineEventFactory *
// Registers known timeline event types in ctor.
// For other types, call Add in game-specific code.
class TimelineEventFactory : public Factory<TimelineEvent>
{
public:
  TimelineEventFactory();
};
using TheTimelineEventFactory = Singleton<TimelineEventFactory>;

// * Timeline *
// Load a timeline of events from file. Each event has a time and a 
//  payload, which could e.g. set an anim on a scene node, 
//  or set some other state.
class Timeline
{
public:
  virtual ~Timeline() = default;

  // Load events; add them to the game message queue in Start.
  bool Load(File* f);

  // Start queueing the events, by setting their start times from
  //  now and adding them to the global MessageQueue.
  // Stop if we get a Wait event. Call again to resume.
  virtual void Start();

  // Reset index so next call to Start restarts the timeline.
  // (Maybe clear message queue if doing this.)
  void Reset() { m_eventIndex = 0; }

protected:
  // By default, calls factory create function.
  // Override to set extra data on timeline event immediately after creation.
  virtual RCPtr<TimelineEvent> CreateTimelineEvent(const std::string& eventType);

  // Name of this timeline, for error reporting, (and also could be so we
  //  can trigger a named timeline)
  std::string m_timelineName;

  // Store the events until we want to start triggering them.
  // This lets us do post-loading processing on each event (e.g. find node
  //  for name, once all nodes are loaded). It also gives us the option
  //  to start the timeline now or later - triggered in code, say, or
  //  by another timeline(!)
  std::vector<RCPtr<TimelineEvent>> m_events;

  // If true, start timeline after loading finished.
  bool m_startNow = false;

  // Index into m_events. If we get a wait event, we save this index,
  //  and resume from it (after the wait event) when resumed.
  int m_eventIndex = 0;
};
}

