#pragma once

#include <SceneGraph.h>

namespace Amju
{
// * GetSceneGraph *
// For all 3D scenes, this is the scene graph
SceneGraph* GetSceneGraph();

// Call to reset e.g. before displaying a new scene
void ResetSceneGraph();
}

