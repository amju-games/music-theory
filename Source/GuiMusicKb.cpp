// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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

//#define MUSIC_KB_DEBUG

// If defined, we allow the user to drag across the keyboard, pressing and releasing
//  keys accordingly.
#define YES_GLISSANDO

//#define YES_ALLOW_SWIPE_TO_SCROLL

//#define DEBUG_SHOW_AABB

namespace Amju
{
const char* GuiMusicKb::NAME = "music-kb";

void GuiMusicKb::Draw()
{
  // After drawing/culling, we know which keys are on screen.
  m_onScreenMin = 127;
  m_onScreenMax = -1;

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
    Key3d* key = dynamic_cast<Key3d*>(pkey.GetPtr());

    AmjuGL::PushMatrix();

    AmjuGL::RotateX(key->m_angle);

    AmjuGL::Scale(sx, 1, sy);
    x += key->m_x;

    Vec2f pos = GetCombinedPos();
    AmjuGL::Translate(pos.x / sx + x, pos.y / sy, 0);

    AmjuGL::SetColour(key->m_colour);

    // Calc projected AABB as a rectangle
    key->CalcRect();

    // Cull off-screen keys; store the range of keys which are on screen.
    if (   key->m_projectedRect.GetMin(0) <  1.f
        && key->m_projectedRect.GetMax(0) > -1.f)
    {
      m_onScreenMin = std::min(m_onScreenMin, key->m_midiNote);
      m_onScreenMax = std::max(m_onScreenMax, key->m_midiNote);
      key->m_mesh->Draw();
    }

#ifdef DEBUG_SHOW_AABB
    AmjuGL::SetColour(Colour(1, 0, 0, 1));
    DrawAABB(key->m_mesh->GetAABB());
#endif

    AmjuGL::PopMatrix();

    // We can bail once we've drawn the rightmost key, no?
    // We have to calc all the projected rects once tho. Hmm.
    /*
    if (key.m_projectedRec.GetMin(0) > 1.f)
    {
      break; 
    }
    */
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

    auto key = new Key3d;
    if (!key->LoadFromString(line))
    {
      f->ReportError("Bad key data.");
      return false;
    }
    
    key->m_midiNote = midiNote;
    midiNote++; 

    m_keys.push_back(key);
  }

  return true;
}

bool GuiMusicKb::Key3d::LoadFromString(const std::string& s)
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

static Vec2f project(const Vec3f& v, const Matrix& m)
{
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

void GuiMusicKb::Key3d::CalcRect()
{
  Matrix modl;
  Matrix proj;
  modl.ModelView();
  proj.Projection();
  Matrix m = modl * proj;

  Vec3f corners[8];
  m_mesh->GetAABB().GetCorners(corners);

  // Project each corner, and enlarge the rectangle to enclose each one
  const float BIG = 99999.9f;
  Rect r(BIG, -BIG, BIG, -BIG);
  for (int i = 0; i < 8; i++)
  {
    Vec2f p = project(corners[i], m);
    r.SetIf(p.x, p.y);
  }

  m_projectedRect = r;
}
}

