// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <algorithm>
#include <DrawAABB.h>
#include <DrawRect.h>
#include <GuiFactory.h>
#include <Matrix.h>
#include <MessageQueue.h>
#include <ResourceManager.h>
#include <StringUtils.h>
#include <Timer.h>
#include "GuiMusicKb.h"
#include "MusicEvent.h"
#include "PlayMidi.h"
#include "TutorialIds.h"
#include "TutorialManager.h"

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

// If defined, we allow the user to drag across the keyboard, pressing and releasing
//  keys accordingly.
#define YES_GLISSANDO

#define YES_ALLOW_SWIPE_TO_SCROLL

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

GuiMusicKb::~GuiMusicKb()
{
  // Make sure all keys which were pressed send final key up events
  ReleaseAllKeys();
}
  
void GuiMusicKb::Draw()
{
  Shader* shader = AmjuGL::GetCurrentShader();
  AmjuGL::UseShader(nullptr);

  AmjuGL::Disable(AmjuGL::AMJU_TEXTURE_2D);
  PushColour();

  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

  AmjuGL::Enable(AmjuGL::AMJU_LIGHTING);

  const float G_AMB = 0.2f;
  const float AMB = 0.6f;
  const float DIFF = 1.0f;
  const float SPEC = 1.0f;
  AmjuGL::DrawLighting(
    AmjuGL::LightColour(G_AMB, G_AMB, G_AMB), // g ambient
    AmjuGL::LightColour(AMB,   AMB,   AMB), // ambient
    AmjuGL::LightColour(DIFF,  DIFF,  DIFF), // diffuse
    AmjuGL::LightColour(SPEC,  SPEC,  SPEC), // spec
    AmjuGL::Vec3(0, 1, 1));

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  // Ortho view
  AmjuGL::SetOrthoProjection(-1.f, 1.f, 1.f, -1.f, 0.1f, 100);
  
  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);
  AmjuGL::PushMatrix();
  AmjuGL::SetIdentity();
  // Look down on keys - TODO CONFIG
  const float EYE_Y = 9.f;
  const float TARGET_Z = -0.55f;
  AmjuGL::LookAt(0, EYE_Y, 10,   0, 0, TARGET_Z,   0, 1, 0);

  float sx = GetSize().x;
  float sy = GetSize().y;
  float x = 0; // cumulative x pos of each key

  for (PKey pkey: m_keys)
  {
    Key& key = *pkey;

    AmjuGL::PushMatrix();

    AmjuGL::RotateX(key.m_angle);

    AmjuGL::Scale(sx, 1, sy);
    x += key.m_x;

    Vec2f pos = GetCombinedPos();
    AmjuGL::Translate(pos.x / sx + x, pos.y / sy, 0);

    AmjuGL::SetColour(key.m_colour);

    key.m_mesh->Draw();

#ifdef DEBUG_SHOW_AABB
    AmjuGL::SetColour(Colour(1, 0, 0, 1));
    DrawAABB(key.m_mesh->GetAABB());
#endif

    // Get projected AABB as a rectangle
    key.CalcRect();

    AmjuGL::PopMatrix();
  }
  m_kbWidth = x;

  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_PROJECTION_MATRIX);
  AmjuGL::PopMatrix();

  AmjuGL::SetMatrixMode(AmjuGL::AMJU_MODELVIEW_MATRIX);

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);

#ifdef DEBUG_SHOW_RECT
  for (PKey pkey : m_keys)
  {
    AmjuGL::SetColour(Colour(0, 0, 1, 1));

    Key& key = *pkey;
    DrawRect(key.m_projectedRect);
  }
#endif // DEBUG_SHOW_RECT

  PopColour();
  AmjuGL::UseShader(shader);
  
  // Draw descendants
  GuiComposite::Draw();
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

  int midiNote = 0;
  if (!f->GetInteger(&midiNote))
  {
    f->ReportError("Expected midi value for leftmost note.");
    return false;
  }

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
    
    key->m_midiNote = midiNote;
    midiNote++; 

    m_keys.push_back(key);
  }

  if (!LoadChildren(f))
  {
    return false;
  }

  QueueFirstTimeMsgs({ TUTORIAL_KB_MIDDLE_C, TUTORIAL_KB_SWIPE }, AMJU_FIRST_TIME_THIS_USER);

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

  // Is black? 1 or 0
  int isBlack = ToInt(strs[1]);
  Assert(isBlack == 0 || isBlack == 1);
  m_isBlack = (isBlack != 0);
  // Colour (black key or white, which we then override to highlight a key, etc)
  float WHITE_VAL = .85f;
  float BLACK_VAL = .1f; // 267f;
  Colour KEY_COLOUR[2] = 
  {
    Colour(WHITE_VAL, WHITE_VAL, WHITE_VAL, 1.f),
    Colour(BLACK_VAL, BLACK_VAL, BLACK_VAL, 1.f)
  };
  m_colour = KEY_COLOUR[isBlack]; ////FromHexString(strs[1]);
  m_naturalColour = m_colour;

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
  if (m_isPressed)
  {
    return;
  }

  m_isPressed = true;
  m_desiredAngle = 5.0f;

  PlayMidi(m_midiNote, MIDI_NOTE_MAX_VOLUME); // ?

  TheMessageQueue::Instance()->Add(new MusicKbMsg(MusicKbEvent(m_midiNote, true)));

#ifdef MUSIC_KB_DEBUG
  std::cout << "Playing note: " << m_midiNote << "\n";
#endif
}

void GuiMusicKb::Key::Release()
{
  if (!m_isPressed)
  {
    return;
  }

  m_isPressed = false;
  m_desiredAngle = 0.0f;

  PlayMidi(m_midiNote, 0); // ?

  TheMessageQueue::Instance()->Add(new MusicKbMsg(MusicKbEvent(m_midiNote, false)));

#ifdef MUSIC_KB_DEBUG
  std::cout << "Releasing note: " << m_midiNote << "\n";
#endif
}

void GuiMusicKb::ReleaseKey(Key* key)
{
  if (key == m_lastKey)
  {
    m_lastKey = nullptr;
  }

  if (key)
  {
    key->Release();
  }
}

void GuiMusicKb::PressKey(Key* key)
{
  if (!key)
  {
    ReleaseAllKeys(); // ?
    return;
  }

  if (m_lastKey == key)
  {
    return;
  }

  ReleaseKey(m_lastKey);

  m_lastKey = key;

  if (IsVisible())
  {
    key->Press();
  }
}

void GuiMusicKb::ReleaseAllKeys()
{
  m_lastKey = nullptr;
  for (PKey pkey : m_keys)
  {
    pkey->Release();
  }
}

int GuiMusicKb::GetMinKey() const
{
  Assert(!m_keys.empty());
  int minKey = m_keys[0]->m_midiNote;
  return minKey;
}

int GuiMusicKb::GetMaxKey() const
{
  Assert(!m_keys.empty());
  int maxKey = m_keys.back()->m_midiNote;
  return maxKey;
}

GuiMusicKb::PKey GuiMusicKb::GetKey(int midiNote)
{
  // Binary search for key with the given midi note value
  //  (keys are in midi note value order)
  auto it = std::lower_bound(m_keys.begin(), m_keys.end(), midiNote,
    [](const PKey& k1, int m) { return k1->m_midiNote < m; }
  );
  if (it == m_keys.end())
  {
    return nullptr;
  }
  PKey key = *it;
  Assert(key);
  // If midiNote value is too low, we get the first key, so check we got the
  //  key we asked for!
  if (key->m_midiNote != midiNote)
  {
    return nullptr;
  }
  return key;
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
  
  // Update descendants
  GuiComposite::Update();
}

bool GuiMusicKb::OnCursorEvent(const CursorEvent& ce)
{
#ifdef YES_ALLOW_SWIPE_TO_SCROLL

  if (m_tapDownScroll)
  {
    // Detect swipe
    const float SWIPE_LIMIT = 0.05f; // TODO CONFIG
    const float SPEED_MULT = 16.0f;
    const float OCTAVE_WIDTH = 0.7f;

    float currentX = GetLocalPos().x;
    float dx = ce.dx; 

    if (fabs(dx) > SWIPE_LIMIT && m_vel.x == 0)
    {
      m_vel = Vec2f(dx * SPEED_MULT, 0);
      if (dx > 0)
      {
        // Swipe right => make lower part of the KB visible
        m_desiredX = currentX + OCTAVE_WIDTH;
        float x = m_kbWidth + GetParent()->GetLocalPos().x + 0.2f;
        m_desiredX = std::min(x, m_desiredX); // so we can't go off bottom end
      }
      else
      {
        // Swipe left => make higher part of the KB visible
        m_desiredX = currentX - OCTAVE_WIDTH;
        float x = GetParent()->GetLocalPos().x + 2.1f;
        m_desiredX = std::max(x, m_desiredX); // so we can't go off top end
      }
    }
    return true;
  }
#endif // YES_ALLOW_SWIPE_TO_SCROLL

#ifdef YES_GLISSANDO
  // Glissando
  if (m_tapDown)
  {
    m_tapDownPos = Vec2f(ce.x, ce.y);
    Key* key = PickKey(m_tapDownPos);
    PressKey(key);
  }
#endif // YES_GLISSANDO

  return false;
}

bool GuiMusicKb::OnMouseButtonEvent(const MouseButtonEvent& mbe)
{
  // TODO CONFIG
  // Anything below this line is treated as a tap on the keyboard.
  // Above this, we use to scroll L/R
  const float KEYBOARD_TOP_Y_COORD = -0.2f;

  if (mbe.button != AMJU_BUTTON_MOUSE_LEFT)
  {
    return false;
  }
  
  // Find key we pressed or released
  
  // Touch down on key or in scroll area?
  if (mbe.isDown && mbe.y >= KEYBOARD_TOP_Y_COORD)
  {
    m_tapDownScroll = true;
  }
  else if (mbe.isDown && mbe.y < KEYBOARD_TOP_Y_COORD)
  {
    m_tapDown = true;

    m_tapDownPos = Vec2f(mbe.x, mbe.y);
    Key* key = PickKey(m_tapDownPos);
    PressKey(key);
  }
  else
  {
    m_tapDown = false;
    m_tapDownScroll = false;
    ReleaseAllKeys(); // safety net
  }

  return false;
}

}

