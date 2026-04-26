// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "SceneNode.h"
#include "Md2Model.h"
#include "Shader.h"
#include "Texture.h"

namespace Amju
{
// * Md2SceneNode * 
// Scene graph node displaying an MD2 animated character etc.
class Md2SceneNode : public SceneNode
{
public:
  static const char* NAME;

  Md2SceneNode();

  void Draw() override;
  void Update() override;
  bool Load(File* f) override;

  void SetAnim(int animId);
  void SetAnim(const std::string& animName);

  bool LoadMd2(const std::string& md2name);
  void SetMd2(Md2Model* model);
  Md2Model* GetMd2();

protected:
  bool LoadEverythingExceptChildren(File* f); 

  bool LoadMd2(File* f); // called from Load - gets md2 name and loads it

  // Load comma-sep list of names of anims which should freeze.
  bool LoadFreezeList(File* f);

  // Load comma-sep list of names of anims which should repeat/loop.
  bool LoadLoopList(File* f);

  // Load initial anim name
  bool LoadInitialAnim(File* f);

protected:
  // No texture data here - we don't know how many textures 

  PMd2Model m_model;
  PShader m_shader; // if null, we use default

private:
  int m_anim;
  // TODO Need next anim ?
  int m_frame;
  int m_nextFrame;
  float m_t; // varies from 0..1
};

class Md2SceneNodeWith1Texture : public Md2SceneNode
{
public:
  static const char* NAME;
  bool Load(File* f) override;
  void Draw() override;

protected:
  PTexture m_tex;
};
}
