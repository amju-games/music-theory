// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include "UseVertexColourShader.h"

namespace Amju
{

void UseVertexColourShader()
{
#if defined(WIN32) || defined(MACOSX)
  // Set shader for desktop GL - fixed function doesn't seem to treat vertex colours the
  //  way we want (i.e. multiply by currently active colour)
  static Shader* sh = AmjuGL::LoadShader("Shaders/" + AmjuGL::GetShaderDir() + "/gui");
  Assert(sh);
  AmjuGL::UseShader(sh);
#endif
}

}

