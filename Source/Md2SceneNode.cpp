// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuFirst.h>
#include <AmjuAssert.h>
#include <AmjuGL.h>
#include <DrawAABB.h>
#include <File.h>
#include <ReportError.h>
#include <ResourceManager.h>
#include <Sign.h>
#include <StringUtils.h>
#include <Timer.h>
#include "Md2SceneNode.h"
#include <AmjuFinal.h>

namespace Amju
{
const char* Md2SceneNode::NAME = "md2";

Md2SceneNode::Md2SceneNode()
{
  m_frame = 0;
  m_nextFrame = 1;
  m_t = 0;
  m_anim = 0;
}

void Md2SceneNode::SetListener(AnimListener* listener)
{
  m_listener = listener;
}

void Md2SceneNode::SetAnim(const std::string& animName)
{
  int anim = m_model->GetAnimationFromName(animName);
  if (anim == -1)
  {
    std::cout << "No anim '" << animName << "' for animated char.\n";
    return; // no such anim
  }
  SetAnim(anim);
}

void Md2SceneNode::SetAnim(int anim)
{
  Assert(anim != -1);

  if (anim == m_anim)
  {
    return;
  }

  m_anim = anim;
  // Blend into first frame of new anim
  m_nextFrame = m_model->GetStartFrame(m_anim);
}

void Md2SceneNode::SetMd2(Md2Model* model)
{
  m_model = model;
}

Md2Model* Md2SceneNode::GetMd2()
{
  return m_model;
}

bool Md2SceneNode::LoadMd2(const std::string& md2name)
{
  m_model = dynamic_cast<Md2Model*>(
    TheResourceManager::Instance()->GetRes(md2name));
  if (!m_model)
  {
    ReportError("Failed to load MD2: " + md2name);
    return false;
  }
  return true;
}

void Md2SceneNode::Draw()
{
  Assert(IsVisible());

  Assert(m_model);

  float t = m_t * 10.0f;
  if (t > 1.0f)
  {
    t = 1.0f;
  }
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);
  // TODO Offset Y so feet are at zero
  AmjuGL::Translate(0, 23.0f, 0);
  m_model->DrawFrames(m_frame, m_nextFrame, t);
  AmjuGL::PopMatrix();
}

void Md2SceneNode::Update()
{
  if (!IsVisible())
  {
    return;
  }

  SceneNode::Update();

  Assert(m_model);
  float dt = TheTimer::Instance()->GetDt();
  m_t += dt;

  if (m_t > 0.1f) // MD2 frames are 0.1 secs each 
  {
    m_t = 0;
    m_frame = m_nextFrame;

    int startFrame = m_model->GetStartFrame(m_anim);
    int size = m_model->GetAnimationSize((Md2Model::Animation)m_anim);

    // If next frame is in the same animation, increment to next frame,
    //  or go back to first frame
    if (m_nextFrame >= startFrame && m_nextFrame <= startFrame + size)
    {
      m_nextFrame++;
      if (m_nextFrame >= startFrame + size)
      {
        if (m_model->GetDoesActionRepeat(m_anim))
        {
          m_nextFrame = startFrame;
          // animation listener mixin gets callback when we repeat, finish anim
          if (m_listener) m_listener->OnAnimRepeat();
        }
        else if (m_model->GetDoesActionFreeze(m_anim))
        {
          m_nextFrame--;
          if (m_listener) m_listener->OnAnimFreeze();
        }
        else
        {
          // TODO This doesn't work, overwritten by other code
          // Doesn't repeat or freeze - go back to stand
          m_anim = 0;
          m_nextFrame = 0;
          if (m_listener) m_listener->OnAnimFinished();
        }
      }
    }
  }
}

bool Md2SceneNode::LoadMd2(File* f)
{
  // Get md2 resource name, and load it
  std::string md2Name;
  if (!f->GetDataLine(&md2Name))
  {
    f->ReportError("Expected .md2 filename");
    return false;
  }
  LoadMd2(md2Name);
  return true;
}

bool Md2SceneNode::LoadEverythingExceptChildren(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected md2 scene node name");
    return false;
  }

  if (!LoadMatrix(f)) return false; 
  if (!LoadMd2(f)) return false; 
  if (!LoadFreezeList(f)) return false;
  if (!LoadLoopList(f)) return false;
  if (!LoadInitialAnim(f)) return false;
  return true;
}

bool Md2SceneNode::Load(File* f)
{
  if (!LoadEverythingExceptChildren(f)) return false;
  if (!LoadChildren(f)) return false; 
  return true;
}

bool Md2SceneNode::LoadInitialAnim(File* f)
{
  std::string initialAnimName;
  if (!f->GetDataLine(&initialAnimName))
  {
    f->ReportError("Expected initial anim name for md2 node.");
    return false;
  }
  auto anim = m_model->GetAnimationFromName(initialAnimName);
  if (anim == -1)
  {
    f->ReportError("Md2: unrecognised anim name for initial anim: " + initialAnimName);
    return false;
  }
  m_frame = m_model->GetStartFrame(anim);
  m_nextFrame = m_frame + 1;
  return true;
}

bool Md2SceneNode::LoadFreezeList(File* f)
{
  std::string list;
  if (!f->GetDataLine(&list))
  {
    f->ReportError("Expected freeze list for md2 node.");
    return false;
  }
  // "none" if none of the anims on this model freeze
  if (list == "none") return true;

  const auto names = Split(list, ',');
  for (const auto& name : names)
  {
    auto anim = m_model->GetAnimationFromName(name);
    if (anim == -1)
    {
      f->ReportError("Md2: unrecognised anim name for freeze list: " + name);
      return false;
    }
    m_model->SetDoesFreeze(anim, true);
  }
  return true;
}

bool Md2SceneNode::LoadLoopList(File* f)
{
  std::string list;
  if (!f->GetDataLine(&list))
  {
    f->ReportError("Expected loop list for md2 node.");
    return false;
  }
  // "none" if none of the anims on this model loop
  if (list == "none") return true;

  const auto names = Split(list, ',');
  for (const auto& name : names)
  {
    auto anim = m_model->GetAnimationFromName(name);
    if (anim == -1)
    {
      f->ReportError("Md2: unrecognised anim name for loop list: " + name);
      return false;
    }
    m_model->SetDoesRepeat(anim, true);
  }
  return true;
}

const char* Md2SceneNodeWith1Texture::NAME = "md2-1tex";

bool Md2SceneNodeWith1Texture::Load(File* f)
{
  if (!LoadEverythingExceptChildren(f)) return false;

  auto rm = TheResourceManager::Instance();
  std::string texName;
  if (!f->GetDataLine(&texName))
  {
    f->ReportError("Expected texture name for md2 with 1 texture.");
    return false;
  }
  m_tex = dynamic_cast<Texture*>(rm->GetRes(texName));

  // TODO Factor this out
  std::string shaderName;
  if (!f->GetDataLine(&shaderName))
  {
    f->ReportError("Expected shader name for md2 with 1 texture.");
    return false;
  }
  // Shader name needs to be able to specify default shader.
  if (shaderName != "default")
  {
    m_shader = dynamic_cast<Shader*>(rm->GetRes(shaderName + ".shader"));
    if (!m_shader)
    {
      f->ReportError("Bad shader for md2 with 1 texture?");
    }
  }

  if (!LoadChildren(f)) return false;

  return true;
}

void Md2SceneNodeWith1Texture::Draw()
{
  // TODO Batch this (batching needs to take shader into account)
  Assert(m_tex);
  m_tex->UseThisTexture();
  if (m_shader)
  {
    PushShader();
    m_shader->UseThisShader();
  }
  Md2SceneNode::Draw();
  if (m_shader)
  {
    PopShader();
  }
}
}
