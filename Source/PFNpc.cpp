#include "PFNpc.h"

namespace Amju
{
void PFNpc::Update() 
{
  Npc::Update();

  auto pos = GetPos();
  if (m_isZFixed)
  {
    pos.z = m_fixedZ;
    SetPos(pos);
  }

  UpdateShadow(pos);
}

void PFNpc::SetFixedZ(bool isFixed, float z) 
{
  m_isZFixed = isFixed; 
  m_fixedZ = z; 
}
}

