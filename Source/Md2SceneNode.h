// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include "SceneNode.h"
#include "Md2Model.h"

namespace Amju
{
// * Md2SceneNode * 
// Scene graph node displaying an MD2 animated character etc.
class Md2SceneNode : public SceneNode
{
public:
  Md2SceneNode();

  virtual void Draw();
  virtual void Update();
  void SetAnim(int animId);
  void SetAnim(const std::string& animName);

  bool LoadMd2(const std::string& md2name);
  void SetMd2(Md2Model* model);
  Md2Model* GetMd2();

protected:
  // No texture data here - we don't know how many textures 

  PMd2Model m_pModel;

private:
  int m_anim;
  // TODO Need next anim ?
  int m_frame;
  int m_nextFrame;
  float m_t; // varies from 0..1
};
}
