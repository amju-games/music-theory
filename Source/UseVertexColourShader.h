// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

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
