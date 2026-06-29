#include "catch.hpp"
#include "LayoutGrid.h"

TEST_CASE("LayoutGrid - Verify Bar Weights Are Order Independent", "[Layout]")
{
  // Common properties for a standard 4/4 bar
  const TimeValue barStart = 0.0f;
  const TimeValue barDuration = 4.0f; // 4 beats total
  const float exponent = 0.5f;

  SECTION("Scenario A: The Crotchet Bar (4 dense notes)")
  {
    // "4/4 <c> c4 e4 g4 c5 |"
    std::vector<TimeValue> crotchetGlyphs = { 0.0f, 1.0f, 2.0f, 3.0f };

    auto attackPoints = LayoutGrid::ExtractAttackPoints(barStart, barDuration, crotchetGlyphs);
    
    // We expect exactly 5 unique boundaries: 0.0, 1.0, 2.0, 3.0, and the end anchor 4.0
    REQUIRE(attackPoints.size() == 5);
    CHECK(attackPoints[0] == 0.0f);
    CHECK(attackPoints[4] == 4.0f);

    // Calculate weight: sqrt(1) + sqrt(1) + sqrt(1) + sqrt(1) = 4.0f
    float weight = LayoutGrid::CalculateWeight(attackPoints, exponent);
    CHECK(weight == Approx(4.0f));
  }

  SECTION("Scenario B: The Semibreve Bar (1 long note)")
  {
    // "4/4 <sb> c5 |"
    std::vector<TimeValue> semibreveGlyphs = { 0.0f };

    auto attackPoints = LayoutGrid::ExtractAttackPoints(barStart, barDuration, semibreveGlyphs);
    
    // We expect exactly 2 unique boundaries: the start 0.0 and the end anchor 4.0
    REQUIRE(attackPoints.size() == 2);
    CHECK(attackPoints[0] == 0.0f);
    CHECK(attackPoints[1] == 4.0f);

    // Calculate weight: sqrt(4) = 2.0f
    float weight = LayoutGrid::CalculateWeight(attackPoints, exponent);
    CHECK(weight == Approx(2.0f));
  }
}

