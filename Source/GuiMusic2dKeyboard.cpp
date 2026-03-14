// * Amjula music theory *
// (c) Copyright 2026 Juliet Colman

#include <array>
#include <DrawRect.h>
#include "GuiMusic2dKeyboard.h"

#define DEBUG_SHOW_RECT

namespace Amju
{
const char* GuiMusic2dKeyboard::NAME = "music-kb-2d";

void GuiMusic2dKeyboard::Draw() 
{
  // Draw all keys (can cull here); as they will be a Batched type,
  //  the whole thing will be one draw call.
  // The rotate angle of each key is mapped to a translation.

  // After drawing/culling, we know which keys are on screen.
  m_onScreenMin = 127;
  m_onScreenMax = -1;

  // TODO Use special shader, e.g. for shiny keys  
//  Shader* shader = AmjuGL::GetCurrentShader();
//  AmjuGL::UseShader(nullptr);

  AmjuGL::PushMatrix();
  const auto pos = GetCombinedPos();
  AmjuGL::Translate(pos.x, pos.y, 0);

  for (auto key : m_keys)
  {
    Key2d* key2d = dynamic_cast<Key2d*>(key.GetPtr());

    auto r = key2d->m_keyComp->CalcRect();
    r.Translate(pos.x, pos.y);
    key2d->m_projectedRect = r;

    // Cull off-screen keys; store the range of keys which are on screen.
    if (   key->m_projectedRect.GetMin(0) <  1.f 
        && key->m_projectedRect.GetMax(0) > -1.f)
    {   
      m_onScreenMin = std::min(m_onScreenMin, key->m_midiNote);
      m_onScreenMax = std::max(m_onScreenMax, key->m_midiNote);
    
      // Draw this key.
      PushColour();
      AmjuGL::PushMatrix();

      AmjuGL::SetColour(key->m_colour);
      AmjuGL::Translate(0, key->m_angle * -0.003f, 0);
      key2d->m_keyComp->Draw();

      PopColour();
      AmjuGL::PopMatrix();
    }   
  }

//  AmjuGL::UseShader(shader);
  AmjuGL::PopMatrix();
}

bool GuiMusic2dKeyboard::Load(File* f) 
{
  if (!GuiElement::Load(f)) // load name, pos, size
  {
    return false;
  }

  // Load keys

  const bool white = false;
  const bool black = true;
  const float w = 0.08f;

  std::array<std::tuple<std::string, bool, float>, 12> KEY_INFOS = 
  {{
    { "Gui/key2d-a.txt", white, w },
    { "Gui/key2d-black.txt", black, w },
    { "Gui/key2d-b-e.txt", white, w },
    { "Gui/key2d-c-f.txt", white, w * 2.f},
    { "Gui/key2d-black.txt", black, w },
    { "Gui/key2d-d.txt", white, w },
    { "Gui/key2d-black.txt", black, w },
    { "Gui/key2d-b-e.txt", white, w },
    { "Gui/key2d-c-f.txt", white, w * 2.f },
    { "Gui/key2d-black.txt", black, w },
    { "Gui/key2d-g.txt", white, w },
    { "Gui/key2d-black.txt", black, w },
  }};

  const float keyWidth = 0.155f; // Get from bounding rect of a key?
  float x = -26.f * keyWidth;
  int midi = 21; // first, lowest midi note: A
  int numNotes = 88;
  for (int i = 0; i < numNotes; i++)
  {
    const auto keyinfo = KEY_INFOS[i % 12];
    auto newKey = new Key2d;
    std::string keyFile = std::get<0>(keyinfo); //"Gui/key2d-c-f.txt";
    bool isBlack = std::get<1>(keyinfo);
    x += std::get<2>(keyinfo);
    
    if (!newKey->Load(keyFile, x, isBlack))
    {
      f->ReportError("Failed to load key " + keyFile);
      return false;
    }
    newKey->m_name = "c";
    newKey->m_displayName = "c";
    newKey->m_midiNote = midi++;

    m_keys.push_back(newKey);
  }

  return true;
}

bool GuiMusic2dKeyboard::Key2d::Load(const std::string& s, float x, bool isBlack)
{
  auto key = LoadGui(s, false);
  Assert(key);
  auto comp = dynamic_cast<GuiComposite*>(key.GetPtr());
  Assert(comp);
  m_keyComp = comp;
  auto pos = m_keyComp->GetLocalPos();
  pos.x += x;
  m_keyComp->SetLocalPos(pos);

  m_isBlack = isBlack;
  // Colour (black key or white, which we then override to highlight a key, etc)
  float WHITE_VAL = .85f;
  float BLACK_VAL = .1f; // 267f;
  Colour KEY_COLOUR[2] = 
  {
    Colour(WHITE_VAL, WHITE_VAL, WHITE_VAL, 1.f),
    Colour(BLACK_VAL, BLACK_VAL, BLACK_VAL, 1.f)
  };  
  m_colour = KEY_COLOUR[isBlack];
  m_naturalColour = m_colour;
  return true;
}

void GuiMusic2dKeyboard::Key2d::CalcRect()
{
}
}

