#pragma once

#include <Texture.h>
#include "Md2SceneNode.h"

namespace Amju
{
// Md2 scene node that switches between two textures, for blinking.
// (Better to do it in shader?)
class BlinkSceneNode : public Md2SceneNode
{
public:
  static const char* NAME;

  BlinkSceneNode();
  void Draw() override;
  void Update() override;
  bool Load(File*) override;

  bool LoadTextures(const std::string& texture1, const std::string& texture2);

protected:
  // 2 textures for blinking 
  PTexture m_pTex[2];
  float m_blinkTime;
  // TODO Each character should blink eyes at different rate
};
}

