#pragma once

#include <SceneGraph.h>

namespace Amju
{
// Note on Scene Graph lifetime
// This scene graph instance lives from the first call to
//  GetScenGraph() until it is destroyed by ResetSceneGraph()
//  or at the end of the process.
// (Don't leave it too late, we don't want a crash on exit :( )

// * GetSceneGraph *
// For all 3D scenes, this is the scene graph.
// We don't need an Init function: we initialise here on the first
//  call or after a reset.
SceneGraph* GetSceneGraph();

// Call to reset, deleting all scene nodes.
void ResetSceneGraph();
}

