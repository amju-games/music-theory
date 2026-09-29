// PIANO FEST 

#include "BassPlayMidi.h" // ..until promoted to amjulib/BassSoundPlayer
#include "UseVertexColourShader.h"

namespace Amju
{
// Shut down all game-specific and amjulib resources to prevent
//  possible crashes on exit.
void ShutDown()
{
  BassMidiShutdown(); // ..until promoted to amjulib/BassSoundPlayer

  DestroyVertexColourShader();
}
}