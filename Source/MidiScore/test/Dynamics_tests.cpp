// * MidiScore *
// (c) Copyright 2026 Juliet Colman

#include "catch.hpp"
#include "Dynamics.h" 
#include "Event.h"

using namespace MidiScore;

// Create one note event 
Event n(int pitch, int vel)
{
  Event e;
  e.m_type = EventType::NOTE;
  e.m_pitch = pitch;
  e.m_velocity = vel;
  return e;
}

TEST_CASE("Dynamics: velocity to string", "[Dynamics]")
{
  // Check some edges, and min and max vel.... what could go wrong?!
  REQUIRE(VelocityToString(-66) == "ppp");
  REQUIRE(VelocityToString(0)   == "ppp");
  REQUIRE(VelocityToString(15)  == "ppp");
  REQUIRE(VelocityToString(16)  == "pp");
  REQUIRE(VelocityToString(31)  == "pp");
  REQUIRE(VelocityToString(32)  == "p");
  REQUIRE(VelocityToString(47)  == "p");
  REQUIRE(VelocityToString(48)  == "mp");
  REQUIRE(VelocityToString(64)  == "mf");
  REQUIRE(VelocityToString(127) == "fff");
  REQUIRE(VelocityToString(666) == "fff");
}

TEST_CASE("Dynamics processing", "[Dynamics]")
{
  Events events;
//  AppendNoteEventToEvents(e, events);

}

