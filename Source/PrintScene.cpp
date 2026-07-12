#include <SceneGraph.h>
#include "Describe.h"
#include "PrintScene.h"

namespace Amju
{
void PrintNode(SceneNode* sn, int depth = 0)
{
  std::string spaces(4 * depth, ' ');
  //std::string type = typeid(sn).name();
  std::cout 
    << spaces << sn->GetName() 
    << "\n"
    //<< " (" << type << ")" // not useful info
    << spaces << "Local: " << Describe(sn->GetLocalTransform()) 
    << "\n"
    << spaces << "Combi: " << Describe(sn->GetCombinedTransform()) 
    << "\n";
  
  int n = sn->GetNumChildren();
  if (n == 0) return;

  std::cout << ": " << n << (n > 1 ? " children" : " child") << ":\n";

  for (int i = 0; i < n; i++)
  {   
    std::cout << spaces << "Child " << i << ":\n";
    PrintNode(sn->GetChild(i), depth + 1); 
  }   
}

void PrintScene(SceneGraph* sg)
{
  auto root = sg->GetRootNode(SceneGraph::AMJU_OPAQUE);
  PrintNode(root);   
}

}

