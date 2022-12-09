// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <LoadScene.h>
#include <ResourceManager.h>
#include <SceneGraph.h>
#include "GSBase3d.h"

namespace Amju
{
SceneGraph* GSBase3d::GetSceneGraph()
{
  static SceneGraph* sg = nullptr;
  if (!sg)
  {
    sg = new SceneGraph;
  }
  return sg;
}

void GSBase3d::Reload3d()
{
  if (m_sceneFilename.empty())
  {
    return;
  }

  SceneGraph* sg = GetSceneGraph();
  sg->Clear(); // for reload

  SceneNode* root = new SceneNode;
  sg->SetRootNode(SceneGraph::AMJU_OPAQUE, root);

  // Load 3D scene. TODO Base class for corridor/menu state and this, in-classroom state.
  PSceneNode node = LoadScene(m_sceneFilename);
  Assert(node);
  root->AddChild(node);
}

void GSBase3d::Draw()
{
  // Cameras set in scene desc file
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  // Use standard/default shader for 3D scene
  AmjuGL::UseShader(nullptr);

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::LightColour ambient(0.6f, 0.6f, 0.6f);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    ambient, // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  GetSceneGraph()->Draw();
}

void GSBase3d::DebugCamera(char key)
{
  const float CAM_POS_CHANGE = 1.f; // change in camera position

  auto root = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE);
  SceneNode* node = root->GetNodeByName("camera");
  SceneNodeCamera* cam = dynamic_cast<SceneNodeCamera*>(node);
  if (!cam)
  {
    return;
  }

  Vec3f eye = cam->GetEyePos();
  switch (key)
  {
  case 'a':
    eye.x -= CAM_POS_CHANGE;
    break;
  case 'd':
    eye.x += CAM_POS_CHANGE;
    break;
  case 'w':
    eye.z -= CAM_POS_CHANGE;
    break;
  case 's':
    eye.z += CAM_POS_CHANGE;
    break;
  case 'q':
    eye.y -= CAM_POS_CHANGE;
    break;
  case 'e':
    eye.y += CAM_POS_CHANGE;
    break;
  }
  cam->SetEyePos(eye);
  std::cout << "Eye pos: " << eye.x << ", " << eye.y << ", " << eye.z << "\n";
}

bool GSBase3d::OnKeyEvent(const KeyEvent& ke)
{
  if (GSBase::OnKeyEvent(ke))
  {
    return true;
  }

#ifdef _DEBUG
  if (ke.keyDown && ke.keyType == AMJU_KEY_CHAR)
  {
    DebugCamera(ke.key);

    switch (ke.key)
    {
    case '3':
      Reload3d();
      break;
    }
  }
#endif // _DEBUG

  return false;
}

void GSBase3d::Update()
{
  GSBase::Update();
  GetSceneGraph()->Update();
}

void GSBase3d::OnActive()
{
  GSBase::OnActive();
  Reload3d();
}

}
