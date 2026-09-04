#pragma once

#include "GSBase3d.h"

namespace Amju
{
// Use this to go to the next game state, via the Dino linking state.
// Dino state plays through the given script, then switches to the 
//  next state.
void GoViaDino(GameState* nextState, const std::string& dinoScript);

// * GSDino *
// Show dino, who talks to the player. Used as conversational links
//  between the other game states.
// So maybe GoToViaDino<STATE>(dinoScript) ???
// We set the next state in GSDino, so when the 'conversation' is over,
//  we go to the next state.
class GSDino : public GSBase3d
{
public:
  GSDino();
  void SetNextState(GameState* gs);

  void OnActive() override;

  void Update() override;

  // Dino 'conversation' is set in a script file.
  // Load a script before switching to this state. We can change scripts 
  // or move from one script to another (conversation tree?!)
  bool LoadScript(const std::string& scriptFile);

  // Set name of script to load once we are active; this avoids a 
  //  race condition where we expect the state to be active and to have
  //  loaded the scene.
  void SetPendingScriptFile(const std::string& scriptFile);

private:
  GameState* m_nextState = nullptr;
  std::string m_pendingScriptFile;
};

using TheGSDino = Singleton<GSDino>;
}

