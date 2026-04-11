// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

namespace Amju
{
// TODO promote to AmjuGL
void PushShader();

// TODO promote to AmjuGL
void PopShader();

// Use shader which correctly combines global and vertex colours - 
//  apparently necessary for desktop OGL?
void UseVertexColourShader();
}

