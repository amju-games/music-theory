// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <LoadScene.h>
#include <ResourceManager.h>
#include "GSBase3d.h"
#include "MySceneGraph.h"
#include "PrintScene.h"

namespace Amju
{
static float xrot = 0; 
static float yrot = 0;
static bool drag = false;

void GSBase3d::OnDeactive()
{
  // DON'T clear the scene graph automatically when we change game
  //  states; reset explicitly if required.
//  GetSceneGraph()->Clear(); 

  GSBase::OnDeactive();
}

void GSBase3d::Reload3d()
{
  // Call Reload3d explicitly if you want to load/reload a scene.
  // But don't call it automatically at the start of a state.
  // We want the 3d scene to live across multiple game states.

  if (m_sceneFilename.empty())
  {
    return;
  }

std::cout << "Loading 3d scene: " << m_sceneFilename << "\n";

  Amju::ResetSceneGraph();
  SceneGraph* sg = GetSceneGraph();

  // Load 3D scene.
  PSceneNode node = LoadScene(m_sceneFilename);
  Assert(node);

  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE);
  root->AddChild(node);
}

void GSBase3d::Draw()
{
  // Cameras set in scene desc file
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  // Use standard/default shader for 3D scene
  AmjuGL::UseShader(nullptr);

  AmjuGL::PushMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float CAM_FOVY = 60.0f;
  const float CAM_NEAR = 1.0f;
  const float CAM_FAR = 3000.0f;
  float aspect = 1.3f;
  AmjuGL::SetPerspectiveProjection(CAM_FOVY, aspect, CAM_NEAR, CAM_FAR);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(0, 10, 100, 0, 0, 0, 0, 1, 0);

  AmjuGL::RotateY(yrot);
  AmjuGL::RotateX(xrot);

  GetSceneGraph()->Draw();
  AmjuGL::PopMatrix();
}

bool GSBase3d::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    drag = mbe.isDown;
  }
  return false;
}

bool GSBase3d::OnCursorEvent(const CursorEvent& ce)
{
  static float oldx = ce.x;
  static float oldy = ce.y;

  if (drag)
  {
    float xdiff = ce.x - oldx;
    yrot += xdiff * 100.0f;

    float ydiff = ce.y - oldy;
    xrot += ydiff * 100.0f;
  }
  oldx = ce.x;
  oldy = ce.y;
  return false;
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
  
    case '4':
      PrintScene(GetSceneGraph());
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

  // DON'T automatically reload the scene: we want to persist the
  //  scene across game states.
//  Reload3d();
}
}

