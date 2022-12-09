#include <AmjuFirst.h>
#include "SceneGraph.h"
#include "SceneMesh.h"
#include <DrawAABB.h>
#include <AmjuFinal.h>

#define USE_VFC
//#define VFC_DEBUG
//#define VFC_DEBUG_TEXT
//#define HIERARCHY_DEBUG

namespace Amju
{
bool SceneGraph::BlendNode::operator<(const BlendNode& bn) const
{
  return m_screenZ > bn.m_screenZ; // so further away drawn first
}

bool DefaultBlendNodeComp(
  const SceneGraph::BlendNode& b1, const SceneGraph::BlendNode& b2)
{
  return (b1 < b2);
}

SceneGraph::SceneGraph()
{
  m_nodesInFrustum = 0;
  m_nodesTotal = 0;
  m_isHeirarchy = false;
  m_blendNodeComp = DefaultBlendNodeComp;
}

PSceneNode SceneGraph::GetRootNode(GraphType gt)
{
  return m_root[gt];
}

void SceneGraph::SetRootNode(GraphType gt, PSceneNode root)
{
  m_root[gt] = root;
}

void SceneGraph::SetCamera(PSceneNodeCamera cameraNode)
{
  m_camera = cameraNode;
}

PSceneNodeCamera SceneGraph::GetCamera()
{
  return m_camera;
}

void SceneGraph::Clear()
{
  for (int i = 0; i < (int)AMJU_MAX_GT; i++)
  {
    m_root[i] = 0;
  }
  m_camera = 0;
  m_nodesInFrustum = 0;
}

void SceneGraph::AddBlendNode(PSceneNode p)
{
  BlendNode bn;
  bn.m_sceneNode = p;

  // Get dist from camera to node p

  const Matrix& mat = p->m_combined;
  Vec3f pos(mat[12], mat[13], mat[14]);

  Vec3f camPos = GetCamera()->GetEyePos(); 

  bn.m_screenZ = (camPos - pos).SqLen(); // ..??

  // Store pos of node for other types of ordering
  bn.m_pos = pos;

  m_blendNodes.push_back(bn);
}

Frustum* SceneGraph::GetFrustum()
{
  return &m_frustum;
}

void SceneGraph::DrawAABBs(SceneNode* node)
{
  if (!node)
  {
    return;
  }

  // Good place to work out total number of nodes because we recurse
  //  over the entire tree - no culling here.
  m_nodesTotal++;

  AmjuGL::PushAttrib(
    AmjuGL::AMJU_LIGHTING | 
    AmjuGL::AMJU_TEXTURE_2D |
    AmjuGL::AMJU_DEPTH_READ |
    AmjuGL::AMJU_DEPTH_WRITE);

  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  AmjuGL::SetColour(Colour(0.5, 0.5, 1, 1.0f));
  DrawAABB(*(node->GetAABB()));
  AmjuGL::PopAttrib();

  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    SceneNode* child = node->m_children[i];
    DrawAABBs(child);
  }
}

void SceneGraph::DrawNode(SceneNode* p)
{
#ifdef HIERARCHY_DEBUG
  const AABB& aabb = *(p->GetAABB());
  Frustum::FrustumResult fr = m_frustum.Intersects(aabb);
  Assert(fr != Frustum::AMJU_OUTSIDE);
#endif // HIERARCHY_DEBUG

  m_nodesInFrustum++;

  if (p && p->IsVisible())
  {
    PushColour();
    MultColour(p->m_colour);

    AmjuGL::PushAttrib(AmjuGL::AMJU_DEPTH_WRITE | AmjuGL::AMJU_DEPTH_READ |
      AmjuGL::AMJU_LIGHTING);

    if (p->IsZReadEnabled())
    {
      AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
    }
    else
    {
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
    } 
    if (p->IsZWriteEnabled())
    {
      AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
    }
    else
    {
      AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
    }
    if (p->IsLit())
    {
      AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
    }
    else
    {
      AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
    }

    p->Draw();
    AmjuGL::PopAttrib();

    if (p->ShowAABB()) 
    {
      AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_TEXTURE_2D);
      AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
      AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
      AmjuGL::SetColour(Colour(0.5, 0.5, 0.5, 1.0f));
      DrawAABB(*(p->GetAABB()));
      AmjuGL::PopAttrib();
    }

    PopColour();
  }
}

void SceneGraph::DrawChildren(
  SceneNode* node, Frustum::FrustumResult fr)
{
  // If a node is not visible, its children should also not be visible. Right ?
  if (!node->IsVisible())
  {
    return;
  }

  Assert(!m_isHeirarchy || fr != Frustum::AMJU_OUTSIDE);

  // If this node is a camera, set the SceneGraph current camera to this..?
  if (node->IsCamera())
  {
    Assert(dynamic_cast<SceneNodeCamera*>(node));
    SetCamera((SceneNodeCamera*)node);
  }

  node->BeforeDraw(); 

  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(node->m_local);

  PushColour();
  MultColour(node->m_colour);

  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& child = node->m_children[i];
    if (!child->IsVisible())
    {
      continue;
    }

    // Get frustum result for child. If child nodes are always contained
    //  within their parents, we only need to recalc if the parent node
    //  is partly in and partly outside the frustum (heirarchical).
    // If not heirarchical, do the frustum test every time.
    Frustum::FrustumResult childFr = fr;
    if (!m_isHeirarchy || fr == Frustum::AMJU_PART_INSIDE)
    {
      // Need to do frustum test for child
      childFr = m_frustum.Intersects(*(child->GetAABB()));
    }

    // TODO If a node is not visible, are all children automatically
    //  invisible too ?
#ifdef USE_VFC
    if (childFr == Frustum::AMJU_OUTSIDE && child->IsVisible())
    {
#ifdef VFC_DEBUG_TEXT
      std::cout << "Culling node " << child.GetPtr() << "\n";
#endif
    }
    else if (child->IsVisible())
#else
    if (child->IsVisible())
#endif
    {
      if (child->IsBlended())
      {
        // ** HACK **
        // Draw with blending turned on
        // This is a quick hack until we have a proper multi-pass renderer
        //AddBlendNode(child);
        AmjuGL::Enable(AmjuGL::AMJU_BLEND);
        DrawNode(child);
        AmjuGL::Disable(AmjuGL::AMJU_BLEND);
      }
      else
      {
        DrawNode(child);
      }
    }

    // Draw children of the child node (even if child is invisible)
    // Only do this if the child node has children of course!
#ifdef USE_VFC
    if (childFr != Frustum::AMJU_OUTSIDE && !child->m_children.empty())
#else
    if (!child->m_children.empty())
#endif
    {
      DrawChildren(child, childFr);
    }
  }  

  PopColour();
  AmjuGL::PopMatrix();

  node->AfterDraw();
}

void SceneGraph::SetBlendNodeComp(BlendNodeComp bnc)
{
  m_blendNodeComp = bnc;
}

void SceneGraph::Draw()
{
  m_nodesInFrustum = 0;
  m_nodesTotal = 0;
  m_blendNodes.clear();

  AmjuGL::PushMatrix();

  // TODO SORT THIS OUT
  // Camera(s) are nodes in the OPAQUE tree ??
  // Multiple cameras are ok (allowing mirrors ?) 
  // The current camera is set when DrawChildren() discovers that the
  // current node is a camera.
  // Not: m_camera->Draw();

  if (m_camera)
  {
    m_camera->Draw();
    // TODO This should go in CameraNode::Draw

    // TODO! This is broken! Test and fix VFC!!!
    m_frustum.Create(); 
  }

  Assert(m_root[AMJU_OPAQUE]);

  SceneNode* node = m_root[AMJU_OPAQUE];
  Assert(node);
  Frustum::FrustumResult fr = m_frustum.Intersects(*(node->GetAABB()));

  if (fr != Frustum::AMJU_OUTSIDE)
  {
    DrawNode(node);
    DrawChildren(node, fr);  
  }
  else if (!m_isHeirarchy)
  {
    // Children may still be in
    DrawChildren(node, fr);  
  }

  // Don't want lighting for sky, shadows or HUD, right ?
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

  // Don't want this either, right ?
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);

  if (m_root[AMJU_SKYBOX])
  {
    // Centre on camera
    if (m_camera)
    {
      // Translate to camera eye pos 
      // (We pop matrix after drawing skybox, no additional push/pop required)
      Vec3f pos = m_camera->GetEyePos();
      AmjuGL::Translate(pos.x, pos.y, pos.z);
    }

    DrawNode(m_root[AMJU_SKYBOX]);
    DrawChildren(m_root[AMJU_SKYBOX], Frustum::AMJU_INSIDE); // TODO
  }

  AmjuGL::PopMatrix();
  AmjuGL::PushMatrix();

  // Do camera transformation again for blended nodes
  // TODO Better to remember the transformation we got the first time, in case
  //  there are multiple cameras ?
  if (m_camera)
  {
    m_camera->Draw();
  }

  // Draw alpha-blended nodes
  std::sort(m_blendNodes.begin(), m_blendNodes.end(), m_blendNodeComp);
  
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  for (BlendNodes::iterator it = m_blendNodes.begin(); it != m_blendNodes.end(); ++it)
  {
    PSceneNode sn = it->m_sceneNode; 

    Frustum::FrustumResult fr = m_frustum.Intersects(*(sn->GetAABB()));
    if (fr == Frustum::AMJU_OUTSIDE)
    {
#ifdef VFC_DEBUG_TEXT
      std::cout << "Culling node " << sn.GetPtr() << "\n";
#endif
    }
    else
    {
      AmjuGL::PushMatrix();
      // Blended nodes may need to mult by their combined transform.
      // Shadows don't need to do this because they use absolute coords.
      sn->BeforeDraw();
      DrawNode(sn);
      sn->AfterDraw();
      AmjuGL::PopMatrix();
    }
  }
  AmjuGL::PopMatrix();

#ifdef VFC_DEBUG_TEXT
  std::cout << "Nodes: " << m_nodesTotal << " in frustum: " << m_nodesInFrustum  << "\n";
#endif 

#ifdef VFC_DEBUG
  AmjuGL::PushMatrix();
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);
  if (m_camera)
  {
    m_camera->Draw();
  }
  DrawAABBs(m_root[AMJU_OPAQUE]);
  AmjuGL::PopMatrix();
#endif 

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);
}

void SceneGraph::UpdateNode(SceneNode* node)
{
  if (node)
  {
    node->Update();
  }
}

void SceneGraph::UpdateChildren(SceneNode* node)
{
  if (!node)
  {
    return;
  }

  unsigned int s = node->m_children.size();
  for (unsigned int i = 0; i < s; i++)
  {
    PSceneNode& child = node->m_children[i];
    UpdateNode(child);
    UpdateChildren(child);
  }  
}

void SceneGraph::Update()
{
  UpdateNode(m_camera);
  UpdateChildren(m_camera);

  Assert(m_root[AMJU_OPAQUE]);
  UpdateNode(m_root[AMJU_OPAQUE]);
  UpdateChildren(m_root[AMJU_OPAQUE]);

  m_root[AMJU_OPAQUE]->CombineTransform();

  m_root[AMJU_OPAQUE]->CalcBoundingVol();

  if (m_root[AMJU_SKYBOX])
  {
    UpdateNode(m_root[AMJU_SKYBOX]);
    UpdateChildren(m_root[AMJU_SKYBOX]);
  }
}
}
