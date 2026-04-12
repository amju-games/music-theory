#pragma once

#include <map>
#include <GameObject.h>
#include <SceneNode.h>
#include "AI.h"

namespace Amju
{
// * Npc *
// Animated (MD2) non-player character.
class Npc : public GameObject
{
public:
  AI* GetAI(const char* aiName);
  void SetAI(AI*);

  void AddAI(AI*); // call to populate map
  void DecideAI(); // gets rank of each AI, picks best - so not virtual
  void Update() override;

  void SetAnim(const std::string& animName);

  // Create scene node and add to the given parent node.
  virtual void CreateSceneNode(PSceneNode parent);
  // Get the scene node created above.
  SceneNode* GetSceneNode() { return m_sceneNode; }

  // Direction: this is desired direction (around y-axis), to which
  //  we turn over time, going the shortest way.
  void SetDir(float degs) { m_dir = degs; }
  float GetDir() const { return m_dir; }

protected:
  using AIs = std::map<std::string, PAI>; // TODO unordered?
  AIs m_ais;
  AI* m_ai = nullptr; // the current AI

  float m_dir = 0; // Direction of movement - DEGREES
  // Direction we currently face -- we will reduce the difference
  //  between m_dirCurrent and m_dir until we face the direction of movement.
  float m_dirCurrent = 0;
 
  // MD2 anim index
  int m_anim;

  PSceneNode m_sceneNode;
};
}

