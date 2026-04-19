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
  m_model = (Md2Model*)TheResourceManager::Instance()->GetRes(md2name);
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
          // TODO animation listener mixin gets callback when we repeat, finish anim, etc
          //Assert(m_gameObj);
          //m_gameObj->OnAnimRepeat();
        }
        else if (m_model->GetDoesActionFreeze(m_anim))
        {
          m_nextFrame--;
          //Assert(m_gameObj);
          //m_gameObj->OnAnimFreeze();
        }
        else
        {
          // TODO This doesn't work, overwritten by other code
          // Doesn't repeat or freeze - go back to stand
          m_anim = 0;
          m_nextFrame = 0;
          //Assert(m_gameObj);
          //m_gameObj->OnAnimFinished();
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

bool Md2SceneNode::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected md2 scene node name");
    return false;
  }

  if (!LoadMatrix(f))
  {
    return false;
  }
 
  if (!LoadMd2(f))
  {
    return false;
  }

  if (!LoadChildren(f))
  {
    return false;
  }

  return true;
}

const char* Md2SceneNodeWith1Texture::NAME = "md2-1tex";

bool Md2SceneNodeWith1Texture::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected md2-1tex scene node name");
    return false;
  }

  if (!LoadMatrix(f))
  {
    return false;
  }
 
  if (!LoadMd2(f))
  {
    return false;
  }

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

  if (!LoadChildren(f))
  {
    return false;
  }

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
