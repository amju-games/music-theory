#include <File.h>
#include <ResourceManager.h>
#include <Timer.h>
#include "BlinkSceneNode.h"

namespace Amju
{
const char* BlinkSceneNode::NAME = "blink";

BlinkSceneNode::BlinkSceneNode()
{
  m_blinkTime = 0;
}

bool BlinkSceneNode::Load(File* f)
{
  if (!LoadEverythingExceptChildren(f)) return false;

  // Get two texture names
  std::string tex[2];
  if (   !f->GetDataLine(&tex[0])
      || !f->GetDataLine(&tex[1]))
  {
    f->ReportError("Blinker: expected two texture names.");
    return false;
  }
  if (!LoadTextures(tex[0], tex[1]))
  {
    f->ReportError("Blinker: failed to load two textures.");
    return false;
  }

  if (!LoadChildren(f)) return false;
  return true;
}

bool BlinkSceneNode::LoadTextures(const std::string& tex1, const std::string& tex2)
{
  m_pTex[0] = dynamic_cast<Texture*>(
    TheResourceManager::Instance()->GetRes(tex1));
  if (!m_pTex[0]) return false;

  m_pTex[1] = dynamic_cast<Texture*>(
    TheResourceManager::Instance()->GetRes(tex2));
  if (!m_pTex[1]) return false;

  return true;
}

void BlinkSceneNode::Update()
{
  if (!IsVisible())
  {
    return;
  }
  
  Md2SceneNode::Update();

  m_blinkTime += TheTimer::Instance()->GetDt();
  if (m_blinkTime > 3.2f)
  {
    m_blinkTime = 0;
  }
}

void BlinkSceneNode::Draw()
{
  int blink = 0;

  // TODO CONFIG ? Or random
  if (m_blinkTime > 3.0f && m_blinkTime <= 3.2f)
  {
    blink = 1;
  }

  m_pTex[blink]->UseThisTexture();

  Md2SceneNode::Draw();
}
}
