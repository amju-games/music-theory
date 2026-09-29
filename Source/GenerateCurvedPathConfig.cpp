#include <AmjuRand.h>
#include "CurvedPath.h"
#include "GenerateCurvedPathConfig.h"

namespace Amju
{
static bool RndBool()
{
  return (RandomInt(2) == 0);
}

static int RndInt(int begin, int end)
{
  if (end < begin) std::swap(end, begin);
  int r = RandomInt(end - begin) + begin;
  return r;
}

void AddSpiral(
  PathConfig& config, float start, float end, bool expand, float scale)
{
  config.spirals.emplace_back(SpiralConfig
  {
    start, end, RndInt(1, 3), scale * Rnd(.2f, .4f), 0, RndBool(), expand
  });
}

void AddLoop(
  PathConfig& config, float start, float end, float scale)
{
  config.loops.emplace_back(LoopConfig
  {
    std::min(start, end), std::max(start, end), scale * Rnd(.3f, .6f),
      RndInt(1, 3), RndBool()
  });
}

void AddFigure8(
  PathConfig& config, float start, float end, float scale)
{
  config.figureEights.emplace_back(Figure8Config
  {
    std::min(start, end), std::max(start, end), scale * Rnd(.3f, .7f),
      RndInt(1, 3)
  });
}

void GenerateCurvedPathConfig(PathConfig& config, float extravagance)
{
  // We use extravagance 0..1 to decide how many loops etc to add to the curve. 

  // Use the distance from start to end of the curve to set the scale for
  //  features along it.
  float scale = std::sqrt((config.startPos - config.endPos).SqLen());

  // Initial spiral
  const bool EXPAND = true;
  const bool SHRINK = false;
  const bool addInitialSpiral = (extravagance > .2f);
  const bool addFinalSpiral = (extravagance > .4f);

  if (addInitialSpiral) AddSpiral(config, 0, Rnd(.2f, .4f), EXPAND, scale);

  if (addFinalSpiral) AddSpiral(config, Rnd(.6f, .8f), 1.f, SHRINK, scale);  

  float numLoopMultiplier = 5.f;  // up to a max of this many loops
  int numLoops = 1 + RandomInt(static_cast<int>(
    std::round(extravagance * numLoopMultiplier)));

  for (int i = 0; i < numLoops; ++i)
  {
    AddLoop(config, Rnd(.3f, .7f), Rnd(.3f, .7f), scale);
  }
  
  // One fig-8 if we are v extravagant
  int numFig8s = (extravagance > .8f) ? 1 : 0;
  for (int i = 0; i < numFig8s; ++i)
  {
    AddFigure8(config, Rnd(.3f, .7f), Rnd(.3f, .7f), scale);
  }
}
}

