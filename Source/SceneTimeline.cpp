#include <iostream>
#include <File.h>
#include <ReportError.h>
#include "Md2SceneNode.h"
#include "SceneTimeline.h"

namespace Amju
{
// Doesn't need to be 'scene-timeline' or similar because there will 
//  only be one timeline type within the scene node factory.
const char* SceneTimeline::NAME = "timeline";

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

  return true;  
}

RCPtr<TimelineEvent> 
SceneTimeline::CreateTimelineEvent(const std::string& eventType) 
{
  // Create the node, then set this node as the root under which
  //  we seach for nodes named in the timeline data. 
  // I.e., the timeline is the root node of everything we animate.
  auto res = Timeline::CreateTimelineEvent(eventType);
  auto sceneTimelineEvent = dynamic_cast<SceneNodeTimelineEvent*>(res.GetPtr());
  if (!sceneTimelineEvent)
  {
    return nullptr;
  }
  sceneTimelineEvent->m_sceneRoot = this;
  return res;
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
std::cout << "EXECUTING SET ANIM TIMELINE EVENT!\n";

  // Set anim on md2 scene node.
  auto md2Node = dynamic_cast<Md2SceneNode*>(m_sceneNode);
  Assert(md2Node); 
  md2Node->SetAnim(m_animName);
}
};

