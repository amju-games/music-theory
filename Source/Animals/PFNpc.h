#pragma once

#include "Npc.h"
#include "SuperSimpleShadow.h"

namespace Amju
{
// * PFNpc *
// Game-specific NPC type.
class PFNpc : public Npc, public SuperSimpleShadowCaster
{
public:
  PFNpc();
  void Update() override;

  // For this game, we want the NPCs to optionally travel in lines 
  //  parallel to the x-axis, in which case  we set the z-coord to 
  //  a fixed value.
  void SetFixedZ(bool isFixed, float z = 0);

protected:
  // Create unique ID, as we are not loading (in which case file
  //  would give ID to each object).
  static int CreateId();

  // Relies on specific scene node names. Loads scene tree,
  //  then finds the md2 and shadow nodes.
  void CreateSceneNode();

protected:
  bool m_isZFixed = false;
  float m_fixedZ = 0;
};
}

