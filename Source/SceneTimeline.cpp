#include <iostream>
#include <File.h>
#include <ReportError.h>
#include "Md2SceneNode.h"
#include "MySceneGraph.h"
#include "SceneTimeline.h"

namespace Amju
{
// Doesn't need to be 'scene-timeline' or similar because there will 
//  only be one timeline type within the scene node factory.
const char* SceneTimeline::NAME = "timeline";

// Ha ha, except for this other timeline type!
// This one appends timeline events to another named SceneTimeline.
const char* SceneTimelineAppend::NAME = "timeline-append";

bool SceneTimelineAppend::Load(File* f) 
{
  if (!Timeline::Load(f))
  {
    return false;
  }

  m_name = m_timelineName;

/*
  // Get name of appendee, which should be a SceneTimeline.
  std::string parentName;
  if (!f->GetDataLine(&parentName))
  {
    f->ReportError("Expected parent SceneTimeline name.");
    return false;
  }

  // Test the name is valid now, and cache node.
  // This is quite game-specific. We can promote SceneTimeline
  //  but not this type.
  auto sg = GetSceneGraph();
  Assert(sg);
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  m_parentNode = root; //root->GetNodeByName(parentName);
  if (!m_parentNode)
  {
    f->ReportError("Couldn't find root node in scene graph");
  }

  // We need to be stored somewhere; let's hang off the parent node
  //  we found..?
  m_parentNode->AddChild(this);
*/

  // Don't load child scene nodes; they should be in the scene 
  //  to which we will be appending our events.

  if (m_startNow)
    Start();

  return true;  
}

SceneNode* SceneTimelineAppend::GetTimelineRoot()
{
  auto sg = GetSceneGraph();
  Assert(sg);
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE);
  Assert(root);
  return root;
}

bool SceneTimeline::Load(File* f) 
{
  if (!Timeline::Load(f))
  {
    return false;
  }

  m_name = m_timelineName;

  if (!LoadChildren(f))
  {
    return false;
  }

  if (m_startNow)
    Start();

  return true;  
}

RCPtr<TimelineEvent> 
SceneTimeline::CreateTimelineEvent(const std::string& eventType) 
{
  // Create the timeline event, then set its root, under which
  //  we seach for nodes named in the timeline data. 
  auto res = Timeline::CreateTimelineEvent(eventType);
  auto sceneTimelineEvent = dynamic_cast<SceneNodeTimelineEvent*>(res.GetPtr());
  if (sceneTimelineEvent)
  {
    sceneTimelineEvent->m_sceneRoot = GetTimelineRoot();
  }
  return res;
}

SceneNode* SceneTimeline::GetTimelineRoot()
{
  // For a Scene Timeline, the timeline is the root node under which
  //  we expect to be able to find any named node. 
  // So the timeline should be the root in its scene file.
  return dynamic_cast<SceneNode*>(this);
}

bool SceneNodeTimelineEvent::Load(File* f)
{
  // Convenience for subclasses: load scene node name
  if (!f->GetDataLine(&m_sceneNodeName))
  {
    f->ReportError("Expected scene node name");
    return false;
  }
  return true;
}

void SceneNodeTimelineEvent::OnAddToMessageQueue()
{
  // Look up 'subject' node from name, (now we have loaded the tree)
  m_sceneNode = m_sceneRoot->GetNodeByName(m_sceneNodeName);
  if (!m_sceneNode)
  {
    ReportError(
      "Can't find node named '" + 
      m_sceneNodeName +
      "' under root node " +
      m_sceneRoot->GetName());
    Assert(false);
  }
}

const char* EventSetAnim::NAME = "set-anim";

bool EventSetAnim::Load(File* f) 
{
  if (!SceneNodeTimelineEvent::Load(f))
  {
    return false;
  }

  // Get anim name -- we set it later in Execute.
  if (!f->GetDataLine(&m_animName))
  {
    f->ReportError("Expected anim name");
    return false;
  }

  return true;
}

void EventSetAnim::Execute()
{
#ifdef TIMELINE_DEBUG
std::cout << "EXECUTING SET ANIM TIMELINE EVENT!\n";
#endif

  // Set anim on md2 scene node.
  auto md2Node = dynamic_cast<Md2SceneNode*>(m_sceneNode);
  Assert(md2Node); 
  md2Node->SetAnim(m_animName);
}
};

