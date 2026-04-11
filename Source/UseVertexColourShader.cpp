// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <Shader.h>
#include "UseVertexColourShader.h"

namespace Amju
{
static std::vector<PShader> s_shaderVec;

// TODO promote to AmjuGL
void PushShader()
{
  s_shaderVec.push_back(AmjuGL::GetCurrentShader());
}

// TODO promote to AmjuGL
void PopShader()
{
  auto shader = s_shaderVec.back();
  s_shaderVec.pop_back();
  AmjuGL::UseShader(shader);
}

void UseVertexColourShader()
{
#if defined(WIN32) || defined(MACOSX)
  // Set shader for desktop GL - fixed function doesn't seem to treat vertex colours the
  //  way we want (i.e. multiply by currently active colour)
  static PShader sh = AmjuGL::LoadShader(
    "Shaders/" + AmjuGL::GetShaderDir() + "/gui");
  Assert(sh);
  AmjuGL::UseShader(sh);
#endif
}

}

