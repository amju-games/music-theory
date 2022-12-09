// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiFactory.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include "GuiMusicKb.h"

namespace Amju
{
static GuiElement* CreateMusicKb()
{
  return new GuiMusicKb;
}

void GuiMusicKb::AddToFactory()
{
  TheGuiFactory::Instance()->Add("music-kb", CreateMusicKb);
}

void GuiMusicKb::Draw()
{
  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  AmjuGL::DrawLighting(
    AmjuGL::LightColour(0.2f, 0.2f, 0.2f),
    AmjuGL::LightColour(0.3f, 0.3f, 0.3f),
    AmjuGL::LightColour(0.7f, 0.7f, 0.3f),
    AmjuGL::LightColour(1.0f, 1.0f, 1.0f),
    AmjuGL::Vec3(1, 1, 1));

    AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
    AmjuGL::SetIdentity();
    // TODO ortho
    float W = 4.f;
    AmjuGL::SetOrthoProjection(-W, W, W, -W, 0.1f, 100);
  
    AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
    AmjuGL::SetIdentity();
    AmjuGL::LookAt(0, 3, 10,   0, 0, 0,   0, 1, 0);

  float x = -5;
  for (Key& key: m_keys)
  {
    AmjuGL::PushMatrix();
    AmjuGL::RotateX(20.0f);
    float s = 0.5f;
    AmjuGL::Scale(s, s, s);
    AmjuGL::Translate(x, 0, 0); // TODO need this as a Key member?
    x += 0.54f;

    AmjuGL::SetColour(key.m_colour);

    key.m_mesh->Draw();
    AmjuGL::PopMatrix();
  }
  PopColour();
}

void GuiMusicKb::Update()
{
}

bool GuiMusicKb::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

  std::string line;
  while (f->GetDataLine(&line))
  {
    if (line == "end")
    {
      break;
    }

    Key key;
    if (!key.LoadFromString(line))
    {
      f->ReportError("Bad key data.");
      return false;
    }

    m_keys.push_back(key);
  }

  return true;
}

bool GuiMusicKb::Key::LoadFromString(const std::string& s)
{
  Strings strs = Split(s, ',');

  // Key mesh
  m_mesh = dynamic_cast<ObjMesh*>(TheResourceManager::Instance()->GetRes(strs[0]));
  if (!m_mesh)
  {
    return false;
  }

  // Colour (black key or white, which we then override to highlight a key, etc)
  m_colour = FromHexString(strs[1]);

  return true;
}

}

