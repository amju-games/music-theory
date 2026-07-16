#include "AnimalController.h"
#include "GSHero.h" // next state
#include "GSInitNewRound.h"

namespace Amju
{
void GSInitNewRound::OnActive()
{
  m_sceneFilename = "Scene/animals-ortho.txt";
  m_guiFilename = "Gui/no_gui.txt";

  GSBase3d::OnActive();

  // Make sure the 3d scene is reset and loaded for animals.
  Reload3d();
  GetAnimalController().Init(); // TODO game round info

  // Immediately go to Hero mode.
  // We could make a bit more out of this state, and show a brief
  //  animation before the game round starts.
  // (Could use timeline if we name the animals nodes in the scene,
  //  or could use AIs.) 
  GoTo<TheGSHero>();
}
}

