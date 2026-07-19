#include "AnimalController.h"
#include "GSHero.h" // next state
#include "GSInitNewRound.h"

namespace Amju
{
void GSInitNewRound::Draw()
{
  // TODO Just don't draw first frame if we do more in this state.
}

void GSInitNewRound::OnActive()
{
  m_sceneFilename = "Scene/animals-ortho.txt";
  m_guiFilename = "Gui/no_gui.txt";

  GSBase3d::OnActive();

  // Make sure the 3d scene is reset and loaded for animals.
  Reload3d();

  // Get current game round, pass to init function.
  const auto& gameRound = TheGSHero::Instance()->GetGameRound();
  GetAnimalController().Init(gameRound);

  // Immediately go to Hero mode.
  // We could make a bit more out of this state, and show a brief
  //  animation before the game round starts.
  // (Could use timeline if we name the animals nodes in the scene,
  //  or could use AIs.) 
  GoTo<TheGSHero>();
}
}

