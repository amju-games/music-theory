// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <LoadScene.h>
#include <SceneGraph.h> // TODO TEMP TEST
#include "GSBase3d.h"
#include "Md2SceneNode.h" // TODO TEMP TEST

namespace Amju
{

// TODO TEMP TEST
static SceneGraph* GetSceneGraph()
{
  static SceneGraph* sg = nullptr;
  if (!sg)
  {
    sg = new SceneGraph;
  }
  return sg;
}

// TODO TEMP TEST
void GSBase3d::Load3d()
{
  SceneGraph* sg = GetSceneGraph();

  // Load 3D scene. TODO Base class for corridor/menu state and this, in-classroom state.
  // TODO TEMP TEST
  PSceneNode node = LoadScene("obj/pz-fence.obj");
  Assert(node);
  node->SetIsLit(true);
  //Matrix m;
  //m.Scale(0.5f, 0.5f, 0.5f);
  //m.TranslateKeepRotation(Vec3f(0, 0, -4)); // TODO TEMP TEST
  //node->SetLocalTransform(m);
  SceneNode* root = new SceneNode;
  sg->SetRootNode(SceneGraph::AMJU_OPAQUE, root);
  root->AddChild(node);

  Md2SceneNode* md2node = new Md2SceneNode;
  md2node->LoadMd2("md2/dino.md2");
  root->AddChild(md2node);

  SceneNodeCamera* camera = new SceneNodeCamera;
  camera->SetEyePos(Vec3f(50, 50, 200));
  camera->SetLookAtPos(Vec3f(50, 20, 0));
  sg->SetCamera(camera);
}

void GSBase3d::Draw()
{
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::SetIdentity();
  const float FOVY = 60.0f;
  const float NEAR_DIST = 1.0f;
  const float FAR_DIST = 3000.0f;

  // Always show the same amount of the world,
  // but distorted if viewport does not have this aspect ratio
  const float ASPECT = 1.3f; // Landscape

  AmjuGL::SetPerspectiveProjection(FOVY, ASPECT, NEAR_DIST, FAR_DIST);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::SetIdentity();

  AmjuGL::PushMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::LightColour ambient(0.6f, 0.6f, 0.6f);
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0, 0, 0),
    ambient, // Ambient light colour
    AmjuGL::LightColour(1, 1, 1), // Diffuse light colour
    AmjuGL::LightColour(1, 1, 1),
    AmjuGL::Vec3(1, 1, 1)); // Light direction

  GetSceneGraph()->Draw();

  AmjuGL::PopMatrix();
}

void DebugCamera(char key)
{
  const float CAM_POS_CHANGE = 1.f; // change in camera position

  auto cam = GetSceneGraph()->GetCamera();
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
  }
#endif // _DEBUG

  return false;
}

void GSBase3d::Update()
{
  GSBase::Update();
  GetSceneGraph()->Update();
}

}
