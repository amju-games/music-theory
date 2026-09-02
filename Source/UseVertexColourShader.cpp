// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <DoOnce.h>
#include <Shader.h>
#include "UseVertexColourShader.h"

namespace Amju
{
static PShader vertexColourShader;

void DestroyVertexColourShader()
{
#if defined(WIN32) || defined(MACOSX)
  // Destroy the shader if loaded below.
  vertexColourShader = nullptr;
#endif
}

void UseVertexColourShader()
{
#if defined(WIN32) || defined(MACOSX)
  // Set shader for desktop GL - fixed function doesn't seem to treat vertex colours the
  //  way we want (i.e. multiply by currently active colour)
  do_once
  {
    vertexColourShader = AmjuGL::LoadShader(
      "Shaders/" + AmjuGL::GetShaderDir() + "/gui");
    Assert(vertexColourShader);
  }

  AmjuGL::UseShader(vertexColourShader);

#endif
}

}

