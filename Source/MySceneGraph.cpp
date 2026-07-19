#include <memory>
#include "MySceneGraph.h"

namespace Amju
{
static std::unique_ptr<SceneGraph> s_sg;

SceneGraph* GetSceneGraph()
{
  if (!s_sg)
  {
    s_sg = std::make_unique<SceneGraph>();
    SceneNode* root = new SceneNode;
    s_sg->SetRootNode(SceneGraph::AMJU_OPAQUE, root);
  }
  return s_sg.get();
}

void ResetSceneGraph()
{
  s_sg.reset();
}
}

