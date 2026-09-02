// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

namespace Amju
{
// Use shader which correctly combines global and vertex colours - 
//  apparently necessary for desktop OGL?
void UseVertexColourShader();

// Destroy the shader loaded in the above function.
// Call this on shutdown to avoid a crash on exit!
void DestroyVertexColourShader();
}
