// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <LoadScene.h>
#include <ResourceManager.h>
#include "GSBase3d.h"
#include "MySceneGraph.h"

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

  // Load 3D scene.
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

  // We are now experimenting with NOT using lighting, just baked colours in textures.
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);

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
