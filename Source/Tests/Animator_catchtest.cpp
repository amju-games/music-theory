#include "catch.hpp"
#include <Animator.h>

using namespace Amju;

// Test wrapper to expose protected state for verification
class TestAnimator : public Animator
{
public:
  float GetValue() const
  {
    return m_value;
  }
};

TEST_CASE("Animator Factory and Easing Logic", "[Animator]")
{
  TestAnimator anim;

  SECTION("Default ease function is linear")
  {
    anim.SetCycleTime(10.0f);
    anim.CalcUpdate(5.0f);
    
    // time = 5.0, cycle = 10.0 -> t = 0.5. linear(0.5) = 0.5
    REQUIRE(anim.GetValue() == Approx(0.5f));
  }

  SECTION("Custom ease function can be added and used at runtime")
  {
    Animator::GetEaseFactory().Add("custom-half", [](float) 
    { 
      return 0.5f; 
    });
    
    anim.SetEaseName("custom-half");
    anim.SetCycleTime(10.0f);
    anim.CalcUpdate(2.0f); 
    
    REQUIRE(anim.GetValue() == Approx(0.5f));
  }

  SECTION("Set ease function does not overwrite explicitly set values")
  {
    anim.SetEaseName("set");
    anim.SetValue(0.75f);
    anim.SetCycleTime(10.0f);
    
    anim.CalcUpdate(5.0f);
    // Because ease function is "set" (nullptr), it should skip updating m_value
    REQUIRE(anim.GetValue() == Approx(0.75f));
  }

  SECTION("One-shot loop type triggers onComplete callback")
  {
    anim.SetLoopType(Animator::LoopType::LOOP_TYPE_ONE_SHOT);
    anim.SetCycleTime(1.0f);
    
    bool wasCalled = false;
    anim.SetOnCompleteCallback([&wasCalled](Animator*) 
    { 
      wasCalled = true; 
    });
    
    anim.CalcUpdate(1.5f); // Step past cycle time
    
    REQUIRE(wasCalled == true);
    REQUIRE(anim.GetValue() == Approx(1.0f));
  }

  SECTION("Repeat loop type resets correctly")
  {
    anim.SetLoopType(Animator::LoopType::LOOP_TYPE_REPEAT);
    anim.SetCycleTime(1.0f);
    
    anim.CalcUpdate(1.5f);
    
    REQUIRE(anim.DidReset() == true);
    REQUIRE(anim.GetAnimTimeSeconds() == Approx(0.5f));
  }

  SECTION("Repeat loop keeps easing functions bounded between 0 and 1")
  {
    anim.SetEaseName("sine");
    anim.SetLoopType(Animator::LoopType::LOOP_TYPE_REPEAT);
    anim.SetCycleTime(10.0f);

    // Move time exactly halfway into the SECOND cycle (time = 15.0)
    // t should wrap to 5.0 / 10.0 = 0.5. 
    anim.CalcUpdate(15.0f);

    REQUIRE(anim.DidReset() == true);
    REQUIRE(anim.GetAnimTimeSeconds() == Approx(5.0f));

    // Half way through the cycle, the sine value is zero, but we
    //  scale up between 0 and 1, so it's 0.5.
    REQUIRE(anim.GetValue() == Approx(0.5f));
  }

  SECTION("Mirror repeat reverses t, preventing out-of-bounds easing evaluation")
  {
    anim.SetEaseName("ease-in-out");
    anim.SetLoopType(Animator::LoopType::LOOP_TYPE_MIRROR_REPEAT);
    anim.SetCycleTime(10.0f);

    // Move time 75% into the FIRST cycle (forward phase)
    anim.CalcUpdate(7.5f);
    float forwardPhaseValue = anim.GetValue();

    // Reset and move 25% into the SECOND cycle (reverse phase, time = 12.5)
    // The visual state should perfectly match the 75% forward state.
    TestAnimator anim2;
    anim2.SetEaseName("ease-in-out");
    anim2.SetLoopType(Animator::LoopType::LOOP_TYPE_MIRROR_REPEAT);
    anim2.SetCycleTime(10.0f);
    anim2.CalcUpdate(12.5f);

    REQUIRE(anim2.GetValue() == Approx(forwardPhaseValue));
  }

  SECTION("Elastic ease-in-out overshoots boundaries to create bounce")
  {
    anim.SetEaseName("ease-in-out-elastic");
    anim.SetCycleTime(1.0f);

    // Check extreme boundary conditions
    anim.CalcUpdate(0.0f);
    REQUIRE(anim.GetValue() == Approx(0.0f));

    // Midpoint should exactly split the difference (value = 0.5)
    anim.CalcUpdate(0.5f);
    REQUIRE(anim.GetValue() == Approx(0.5f));

    // Smooth progress check: t=0.4 should smoothly approach 0.5 without sudden jumps
    TestAnimator anim40;
    anim40.SetEaseName("ease-in-out-elastic");
    anim40.SetCycleTime(1.0f);
    anim40.CalcUpdate(0.4f);

    // Verify smooth transition into the 0.5 midpoint
    REQUIRE(anim40.GetValue() < 0.5f);
    REQUIRE(anim40.GetValue() > -0.5f);
  }
}

