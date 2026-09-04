#pragma once

#include <SceneNode.h>
#include "Timeline.h"

namespace Amju
{
// * Scene Timeline *
// Scene node which loads a timeline.
class SceneTimeline : public SceneNode, public Timeline
{
public:
  static const char* NAME;

  // Override SceneNode load: don't load matrix; DO load Timeline.
  bool Load(File*) override;

protected:
  // Create the node, then set this node as the root under which
  //  we seach for nodes named in the timeline data. 
  // I.e., the timeline is the root node of everything we animate.
  RCPtr<TimelineEvent> CreateTimelineEvent(const std::string& eventType) override;

  // Return the root node under which all nodes named in the timeline
  //  should live.
  virtual SceneNode* GetTimelineRoot();
};

// * Scene Timeline Append *
// This special scene timeline appends its events to an existing
//  SceneTimeline.
class SceneTimelineAppend : public SceneTimeline
{
public:
  static const char* NAME;

  // Override load to append to a named Scene Timeline node.
  bool Load(File*) override;

protected:
  // Return root node named in Load data.
  SceneNode* GetTimelineRoot() override;

protected:
  // The parent Scene Timeline to which we will append events.
  SceneNode* m_parentNode = nullptr;
};

// * Scene Node Timeline Event *
// SceneNodeTimelineEvents are messages which act on scene nodes.
struct SceneNodeTimelineEvent : public TimelineEvent
{
  // Convenience for subclasses: load scene node name
  bool Load(File*) override;

  // Convenience for subclasses: find node in scene graph from root
  void OnAddToMessageQueue() override;

  // Set by SceneTimeline which creates this.
  // Use this root node to find named descendant nodes.
  SceneNode* m_sceneRoot = nullptr; 

  std::string m_sceneNodeName;

  // The node on which we will apply state change (e.g. setting anim):
  // Set this in Load() by searching from root for given
  //  node name.
  SceneNode* m_sceneNode = nullptr; 
};

// A SceneNodeTimelineEvent which sets the animation on an MD2 scene node.
struct EventSetAnim : public SceneNodeTimelineEvent
{
  static const char* NAME;

  std::string m_animName;

  // Load scene node name; find node; check it is an MD2; load anim name.
  bool Load(File*) override;

  // Set anim on md2 scene node.
  void Execute() override;
};
}

