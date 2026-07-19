#pragma once

#include "PFNpc.h"

namespace Amju
{
// * Pet *
// Base class for animal types that are not Dinos, to collect
//  common behaviour etc.
class Pet : public PFNpc
{
public:
  Pet();

  void SetIndex(int i) { m_index = i; }
  int GetIndex() const { return m_index; }

protected:
  int m_index = -1;
};
}

