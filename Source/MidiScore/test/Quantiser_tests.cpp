#include "catch.hpp"
#include "Event.h"
#include "Quantiser.h"

using namespace MidiScore;

// Create one note event 
static Event n(int pitch, int start, int duration, int tpq) 
{
  Event e;
  e.m_type = EventType::NOTE;
  e.m_pitch = pitch;
  e.m_unquantisedStart = start;
  e.m_start = start; 
  e.m_unquantisedDuration = duration;
  e.m_duration = duration;
  e.m_end = start + duration;
  e.SetTimeVal(tpq);
  return e;
}

TEST_CASE("Quantise duration", "[Quantiser]")
{
  // Quantise duration, which must be the given resolution at least.
  // Quantising gets rid of weird durations, like SetTimeVal, but
  //  with a selectable resolution, rather than getting the closest,
  //  smallest value.
  
  const int pitch = 60; // not important
  const int tpq = 64;

  SECTION("MonoQuantiser")
  {
    auto e = n(pitch, 73, 9, tpq);
    REQUIRE(e.m_unquantisedStart == 73); 
    REQUIRE(e.m_start == 73); 
    REQUIRE(e.m_unquantisedDuration == 9);
    REQUIRE(e.m_duration == 8); // SetTimeVal quantised it to QQQ

    MonoQuantiser q(TimeVal::CROTCHET);
    q.QuantiseDuration(tpq, e);

    // Duration can't be smaller than quatisation resolution, because
    //  it would be zero.
    REQUIRE(e.m_duration == 64); 
  }
}

TEST_CASE("Quantise start time", "[Quantiser]")
{
  // Quantise event start times with different quant resolutions

  const int pitch = 60; // not important
  const int duration = 1; // not important
  const int tpq = 64;

  SECTION("MonoQuantiser - round down")
  {
    auto e = n(pitch, 73, duration, tpq);

    MonoQuantiser q(TimeVal::CROTCHET);
    q.QuantiseDuration(tpq, e);

    REQUIRE(e.m_unquantisedStart == 73);
    REQUIRE(e.m_start == 73);
    q.QuantiseStartTime(tpq, e);
    REQUIRE(e.m_start == 64);

    // It's ok to repeatedly call Quantise on the same event, because
    //  we retain the unquantised start.
    REQUIRE(e.m_unquantisedStart == 73);
    
    MonoQuantiser q2(TimeVal::QQQ);
    // QQQ duration is 8/64 ticks

    q2.QuantiseStartTime(tpq, e);
    REQUIRE(e.m_start == 72);
  }

  // Check that a start time just under a multiple gets rounded up:
  //  71 should be quantised to 72, not 64.
  SECTION("MonoQuantiser - round up")
  {
    auto e = n(pitch, 71, duration, tpq);

    MonoQuantiser q(TimeVal::QQQ);
    q.QuantiseStartTime(tpq, e);
    REQUIRE(e.m_start == 72);

    // Quantise to semiquaver, now lower time is closer
    MonoQuantiser q2(TimeVal::SEMIQUAVER);
    q2.QuantiseStartTime(tpq, e);
    REQUIRE(e.m_start == 64);
  }
}

