#include <iostream>
#include "Md2SceneNode.h"

namespace Amju
{
void ListAnims(Md2SceneNode* node)
{
  auto model = node->GetMd2();
  const int n = model->GetNumAnimations();
 
  std::cout << "MD2 node " << node->GetName() << " anims:\n"; 
  for (int i = 0; i < n; i++)
  {
    std::cout << "  " << model->GetAnimationName(i);
  }
  std::cout << "\n";
}
}

