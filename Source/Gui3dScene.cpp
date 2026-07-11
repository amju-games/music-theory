// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <GuiFactory.h>
#include <LoadScene.h>
#include <Shader.h>
#include "Gui3dScene.h"
#include "UseVertexColourShader.h"

namespace Amju
{
const char* Gui3dScene::NAME = "3d-scene";

SceneGraph* Gui3dScene::GetSceneGraph()
{
  return m_sceneGraph;
}

void Gui3dScene::Draw()
{
  Assert(m_sceneGraph);

  if (!IsVisible())
  {
    return;
  }

  const Vec2f& pos = GetCombinedPos();
  const Vec2f& size = GetSize();

  PushShader();

  AmjuGL::PushAttrib(
    AmjuGL::AMJU_LIGHTING |
    AmjuGL::AMJU_TEXTURE_2D |
    AmjuGL::AMJU_DEPTH_READ |
    AmjuGL::AMJU_DEPTH_WRITE);

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::PushMatrix();

  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);

  // Turn on depth testing for the 3D mesh
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);

  m_sceneGraph->Draw();

  AmjuGL::PopMatrix();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PopMatrix();
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

  AmjuGL::PopAttrib();

  PopShader();
}

void Gui3dScene::Update()
{
  Assert(m_sceneGraph);
  m_sceneGraph->Update();
}

bool Gui3dScene::Load(File* f)
{
  // Just load name: pos and size are not used.
  // If we want to set up a viewport, use a GuiWindow parent.
  // (TODO test that actually works)

  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Gui3dScene: expected name");
    return false;
  }

  if (!f->GetDataLine(&m_sceneFilename))
  {
    f->ReportError("Expected scene filename");
    return false;
  }
  PSceneNode node = LoadScene(m_sceneFilename);
  if (!node)
  {
    f->ReportError("Failed to load scene " + m_sceneFilename);
    return false;
  }
  m_sceneGraph = new SceneGraph;
  SceneNode* root = new SceneNode;
  m_sceneGraph->SetRootNode(SceneGraph::AMJU_OPAQUE, root);
  root->AddChild(node);

  return true;
}

}
