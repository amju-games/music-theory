#include <iostream>
#include <AmjuAssert.h>
#include "MultiExtra.h"

namespace Amju
{
void MultiExtra::Collect() 
{
  // Collect the multi extra! This might not need to be overridden.
  Extra::Collect();
}

void MultiExtra::StartNoCollect() 
{
std::cout << "No collect on multi extra..\n";
  // No-collect this and all children.
  Extra::StartNoCollect(); 

  for (auto child : m_children)
  {
    if (child->IsActive())
    {
      child->StartNoCollect();
    }
  }
}

void MultiExtra::AddChild(PExtra child)
{
  // Add child extra, so we can notify them in StartNoCollect.
  m_children.push_back(child);
}

void ChildExtra::Collect()
{
  // TODO Boost parent points?
  Extra::Collect();
}

void ChildExtra::StartNoCollect() 
{
std::cout << "No collect on child  extra..\n";

  // Child not collected: notify the parent so we no-collect all the
  //  children and the parent.
  Assert(m_parent);
  bool isParentActive = m_parent->IsActive();

  Extra::StartNoCollect();

  if (isParentActive)
  {
    m_parent->StartNoCollect();
  }
}
}

