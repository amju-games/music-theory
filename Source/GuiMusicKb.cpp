// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <DrawAABB.h>
#include <DrawRect.h>
#include <GuiFactory.h>
#include <Matrix.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include <Timer.h>
#include "GuiMusicKb.h"
#include "iOSPlayMidi.h"

namespace Amju
{
const int MIDI_NOTE_MAX_VOLUME = 127;
  
static GuiElement* CreateMusicKb()
{
  return new GuiMusicKb;
}

void GuiMusicKb::AddToFactory()
{
  TheGuiFactory::Instance()->Add("music-kb", CreateMusicKb);
}

GuiMusicKb::~GuiMusicKb()
{
  // Make sure all keys which were pressed send final key up events
  ReleaseAllKeys();
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
  float x = GetLocalPos().x; 

  for (PKey pkey: m_keys)
  {
    Key& key = *pkey;

    AmjuGL::PushMatrix();

    AmjuGL::RotateX(key.m_angle);

    AmjuGL::Scale(s, s, s);
    x += key.m_x;

    Vec2f pos = GetCombinedPos();
    AmjuGL::Translate(pos.x / s + x, pos.y / s, 0);

    AmjuGL::SetColour(key.m_colour);

    key.m_mesh->Draw();

    AmjuGL::SetColour(Colour(1, 0, 0, 1));

    // Get projected AABB as a rectangle
    key.CalcRect();

    AmjuGL::PopMatrix();
  }
  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

  //for (PKey pkey : m_keys)
  //{
  //  AmjuGL::SetColour(Colour(0, 0, 1, 1));

  //  Key& key = *pkey;
  //  DrawRect(key.m_projectedRect);
  //}

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

  PopColour();
}

GuiMusicKb::PKey GuiMusicKb::PickKey(const Vec2f& pos)
{
  std::vector<PKey> pickedKeys; // should be 0, 1 or 2, right?!
  for (PKey& key : m_keys)
  {
    if (key->m_projectedRect.IsPointIn(pos))
    {
      pickedKeys.push_back(key); 
    }
  }

  int n = pickedKeys.size();
  if (n == 0)
  {
    return nullptr;
  }
  if (n == 1)
  {
    return pickedKeys[0];
  }

  // Decide which one to return: for our keyboard, the black key wins
  for (PKey& key : pickedKeys)
  {
    if (key->m_isBlack)
    {
      return key;
    }
  }

  // No key picked
  return nullptr;
}

bool GuiMusicKb::Load(File* f)
{
  if (!GuiElement::Load(f))
  {
    return false;
  }

  int i = 50; // TODO TEMP TEST
  std::string line;
  while (f->GetDataLine(&line))
  {
    if (line == "end")
    {
      break;
    }

    PKey key = new Key;
    if (!key->LoadFromString(line))
    {
      f->ReportError("Bad key data.");
      return false;
    }
    
    key->m_midiNote = i++; // TODO TEMP TEST

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

static Vec2f _project(const Vec3f& v)
{
  Matrix modl;
  Matrix proj;
  modl.ModelView();
  proj.Projection();
  Matrix m = modl * proj;
  float v4[4] = 
  {
    m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12],
    m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13],
    m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14],
    m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15]
  };

  Vec2f res(v4[0] / v4[3], v4[1] / v4[3]);
  return res;
}

void GuiMusicKb::Key::CalcRect()
{
  Vec3f corners[8];
  m_mesh->GetAABB().GetCorners(corners);

  // Project each corner, and enlarge the rectangle to enclose each one
  const float BIG = 99999.9f;
  Rect r(BIG, -BIG, BIG, -BIG);
  for (int i = 0; i < 8; i++)
  {
    Vec2f p = _project(corners[i]);
    r.SetIf(p.x, p.y);
  }

  m_projectedRect = r;
}

void GuiMusicKb::Key::Press()
{
  m_isPressed = false;
  m_desiredAngle = 5.0f;
  
  PlayMidi(m_midiNote, MIDI_NOTE_MAX_VOLUME);
}
  
void GuiMusicKb::Key::Release()
{
  m_isPressed = false;
  m_desiredAngle = 0.0f;
  
  PlayMidi(m_midiNote, 0);
}

void GuiMusicKb::ReleaseAllKeys()
{
  for (PKey pkey : m_keys)
  {
    pkey->Release();
  }
}
  
void GuiMusicKb::Update()
{
  // Scroll keyboard left/right if swiped
  Vec2f pos = GetLocalPos();
  float dt = TheTimer::Instance()->GetDt();
  pos += m_vel * dt;

  if (   (m_vel.x > 0 && pos.x > m_desiredX) 
      || (m_vel.x < 0 && pos.x < m_desiredX))
  {
    m_vel.x = 0;
    pos.x = m_desiredX;
  }

  SetLocalPos(pos);

  // Update key angles
  const float m_rotVel = 90.0f;
  for (PKey pkey : m_keys)
  {
    if (pkey->m_angle < pkey->m_desiredAngle)
    {
      pkey->m_angle += m_rotVel * dt;
      if (pkey->m_angle > pkey->m_desiredAngle)
      {
        pkey->m_angle = pkey->m_desiredAngle;
      }
    }
    else if (pkey->m_angle > pkey->m_desiredAngle)
    {
      pkey->m_angle -= m_rotVel * dt;
      if (pkey->m_angle < pkey->m_desiredAngle)
      {
        pkey->m_angle = pkey->m_desiredAngle;
      }
    }
  }
}

bool GuiMusicKb::OnCursorEvent(const CursorEvent& ce)
{
  if (m_tapDown)
  {
    // Detect swipe
    const float SWIPE_LIMIT = 0.1f; // TODO CONFIG
    const float SPEED_MULT = 20.0f;
    const float OCTAVE_WIDTH = 2.5f;

    float currentX = GetLocalPos().x;
    float dx = ce.dx; 
//    std::cout << "dx: " << dx << "\n";
    if (fabs(dx) > SWIPE_LIMIT && m_vel.x == 0)
    {
      m_vel = Vec2f(dx * SPEED_MULT, 0);
      if (dx > 0)
      {
        m_octave++;
        m_desiredX = currentX + OCTAVE_WIDTH;
      }
      else
      {
        m_octave--;
        m_desiredX = currentX - OCTAVE_WIDTH;
      }

      std::cout << "Vel is now: " << m_vel.x << "\n";
      std::cout << "New octave: " << m_octave << "\n";
      //m_desiredX = m_x + 2.0f * dx;
    }
    return true;
  }
  return false;
}

bool GuiMusicKb::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // TODO CONFIG
  // Anything below this line is treated as a tap or swipe on the keyboard
  const float KEYBOARD_TOP_Y_COORD = -0.0f;

  if (mbe.button == AMJU_BUTTON_MOUSE_LEFT)
  {
    // Find key we pressed or released

    if (mbe.isDown && mbe.y < KEYBOARD_TOP_Y_COORD)
    {
//      std::cout << "Key down!\n";

      m_tapDown = true;

      m_tapDownPos = Vec2f(mbe.x, mbe.y);
      Key* key = PickKey(m_tapDownPos);
      if (key)
      {
        key->Press();
        // Send key down event
      }
    }
    else
    {
//      std::cout << "Key up!\n";
      m_tapDown = false;
      ReleaseAllKeys(); // safety net
  
      // For polyphonic kb
//      Key* key = PickKey(Vec2f(mbe.x, mbe.y));
//      if (key)
//      {
//        key->Release();
//        // Send key up event
//      }
    }
  }

  return false;
}

}

