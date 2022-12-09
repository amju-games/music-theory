// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <AmjuGL.h>
#include <GuiFactory.h>
#include <LoadScene.h>
#include "Gui3dScene.h"

namespace Amju
{
const char* Gui3dScene::NAME = "3d-scene";

static GuiElement* CreateGui3dScene()
{
  return new Gui3dScene;
}

void Gui3dScene::AddToFactory()
{
  TheGuiFactory::Instance()->Add(NAME, CreateGui3dScene);
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

  AmjuGL::PushMatrix();
  AmjuGL::Translate(pos.x, pos.y, 0);
  AmjuGL::Scale(size.x, size.y, 1);

  // Turn on depth testing for the 3D mesh
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE);

  m_sceneGraph->Draw();

  // Disable depth test again, as we are in GUI rendering
  //AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  AmjuGL::PopMatrix();
}

void Gui3dScene::Update()
{
  Assert(m_sceneGraph);
  m_sceneGraph->Update();
}

bool Gui3dScene::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
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
