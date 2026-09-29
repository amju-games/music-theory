#include "catch.hpp"
#include "Hud.h"
#include "HudNumber.h"

using namespace Amju;

TEST_CASE("Hud: add points", "[HUD]")
{
  Hud hud;
  REQUIRE(hud.GetPlayerScore().m_internalNumber == 0);

//void MultPointsMultiplier(int mult);

  const int FRAMES = 2;
  hud.AddToPlayerPoints(1, FRAMES);

  // Internal number is immediately updated
  REQUIRE(hud.GetPlayerScore().m_internalNumber == 1);
  // Displayed number updates over FRAMES updates
  REQUIRE(hud.GetPlayerScore().m_displayedNumber == 0);

/*
  // ARGH - tricky to test, Hud and HudNumber are deeply entagled
  //  with guis.
  // We have to set up a mock gui tree. TODO
  for (int i = 0; i < FRAMES; i++)
  {
    hud.Update();
  }
  // Same internal number
  REQUIRE(hud.GetPlayerScore().m_internalNumber == 1);
  // Now displayed number matches
  REQUIRE(hud.GetPlayerScore().m_displayedNumber == 1);
*/
}

TEST_CASE("Hud: add points with multiplier", "[HUD]")
{
/*
  Hud hud;

  // TODO Argh we can't test this either, without mocking a gui.
  hud.MultPointsMultiplier(3);

  const int FRAMES = 2;
  hud.AddToPlayerPoints(5, FRAMES);

  REQUIRE(hud.GetPlayerScore().m_internalNumber == 15);
*/
}

