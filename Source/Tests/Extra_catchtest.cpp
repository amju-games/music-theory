#include "catch.hpp"
#include <GuiComposite.h>
#include "ExtrasAdder.h"
#include "FindSongSections.h"
#include "GuiMusicScore.h"
#include "MultiExtra.h"

using namespace Amju;

namespace Amju
{
// Override the bits that assume a running game
class RewardPointsMock : public RewardPoints
{
public:
  RewardPointsMock(int points) : RewardPoints(points) {}

  void InitAnims() override {}
  Vec2f GetCollectDestPos() const override { return {}; }  
};

class RewardPointsChildMock : public RewardPointsChild
{
public:
  RewardPointsChildMock(int points, PGuiElement multiGui) :
    RewardPointsChild(points, multiGui, {}) {}

  void InitAnims() override {}
  Vec2f GetCollectDestPos() const override { return {}; }  
};
}

TEST_CASE("Create multi extra with children", "[Extra]")
{
  // Create multi extra (the 'parent')
  PGuiElement gui = new GuiComposite;
  const int POINTS = 50;
  PReward multiReward = new RewardPointsMock(POINTS);
  RCPtr<MultiExtra> multi = new MultiExtra(gui, multiReward);

  // Create child extras
  std::vector<RCPtr<ChildExtra>> children;
  const int NUM_CHILDREN = 3; 
  for (int i = 0; i < NUM_CHILDREN; ++i)
  { 
    PReward childReward = new RewardPointsChildMock(POINTS, gui);
    RCPtr<ChildExtra> child = new ChildExtra(gui, childReward, multi);
    children.push_back(child);
  }

  // Triggering no-collect on any of the children or the parent
  //  causes the whole lot to be no-collected.

  // Triggering collect on the final parent will collect it only if
  //  no-collect has not already been triggered. So all the children
  //  must have already been collected.
}

