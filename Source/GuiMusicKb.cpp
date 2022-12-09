// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiFactory.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include <Timer.h>
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
    AmjuGL::LightColour(0.7f, 0.7f, 0.7f),
    AmjuGL::LightColour(1.0f, 1.0f, 1.0f),
    AmjuGL::Vec3(0, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  // Ortho view
  float W = 4.f;
  AmjuGL::SetOrthoProjection(-W, W, W, -W, 0.1f, 100);
  
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  AmjuGL::LookAt(0, 6, 10,   0, 0, 0,   0, 1, 0);

  float s = GetSize().x;

  float x = 0; //// -W / s + 0.05f; // left edge

  int i = 0;
  for (Key& key: m_keys)
  {
    AmjuGL::PushMatrix();

    //i++;
    //static float t = 0;
    //t += TheTimer::Instance()->GetDt();
    //if (i == 5 || i == 9)
    //{
    //  float a = fabs(sin(t * 0.1f)) * 5.0f;
    //  AmjuGL::RotateX(a);
    //}

    AmjuGL::RotateX(key.m_angle);

    AmjuGL::Scale(s, s, s);
    x += key.m_x;

    Vec2f pos = GetCombinedPos();
    AmjuGL::Translate(pos.x / s + x, pos.y / s, 0);

    AmjuGL::SetColour(key.m_colour);

    key.m_mesh->Draw();
    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

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

  // x position
  m_x = ToFloat(strs[2]);

  return true;
}

bool GuiMusicKb::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    // Find key we pressed or released

    if (mbe.isDown)
    {
      std::cout << "Key down!\n";
    }
    else
    {
      std::cout << "Key up!\n";
    }
  }

  return false;
}

}

