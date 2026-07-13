#pragma once

#include "Npc.h"
#include "SuperSimpleShadow.h"

namespace Amju
{
// * PFNpc *
// Game-specific NPC type.
// For this game, we want the NPCs to optionally travel in lines parallel
//  to the x-axis, so we set the z-coord to a fixed value.
class PFNpc : public Npc, public SuperSimpleShadowCaster
{
public:
  void Update() override;

  void SetFixedZ(bool isFixed, float z = 0);

protected:
  void CreateSceneNode();

protected:
  bool m_isZFixed = true;
  float m_fixedZ = 0;
};
}

