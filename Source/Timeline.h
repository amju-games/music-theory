#pragma once

#include <Factory.h>
#include <MessageQueue.h>

// For title and other scripted animations, this is a Timeline
//  we can load in from a file. The timeline has messages, each of which
//  executes at a scripted time from when loaded.
namespace Amju
{
class File;

struct TimelineEvent : public Message
{
  virtual ~TimelineEvent() = default;

  virtual bool Load(File*) { return true; }

  void SetTimelineName(const std::string& s) { m_timelineName = s; }

  std::string m_timelineName;
};

/*
// TODO This goes in TimelineSceneNode or SceneNodeTimeline 

// Events are messages which act on a scene node..?
struct SceneNodeTimelineEvent : public TimelineEvent
{
  SceneNode* m_sceneNode = nullptr;
};

struct EventSetAnim : public SceneNodeTimelineEvent
{
  std::string m_animName;

  // Example:
  void Execute() override
  {
    auto md2Node = dynamic_cast<Md2SceneNode*>(m_sceneNode);
    Assert(md2Node);
    md2Node->SetAnim(m_animName);
  }
};
*/

using TimelineEventFactory = Factory<TimelineEvent>;
using TheTimelineEventFactory = Singleton<TimelineEventFactory>;

// * Timeline *
// Load a timeline of events from file. Each event has a time and a 
//  payload, which sets an anim on a scene node, or sets some other state.
class Timeline
{
public:
  // Load events; add them to the game message queue.
  bool Load(File* f);
};
}

