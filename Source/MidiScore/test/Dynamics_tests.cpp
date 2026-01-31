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
  e.m_dynamics.SetVelocity(vel);
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

TEST_CASE("Dynamics processing: big range", "[Dynamics]")
{
  Events events;
  // Add some notes with velocities - check vel and velChange are set
  AppendNoteEventToEvents(n(60, 1), events); // ppp
  AppendNoteEventToEvents(n(62, 125), events); // fff 
  AppendNoteEventToEvents(n(64, 64), events); // mf
  AppendNoteEventToEvents(n(65, 48), events); // mp
  AppendNoteEventToEvents(n(67, 31), events); // pp
  AppendNoteEventToEvents(n(69, 31), events); // pp

  InsertDynamics(events);

  REQUIRE(events[0].m_dynamics.m_velocity == 1);
  REQUIRE(events[1].m_dynamics.m_velocity == 125);

  REQUIRE(events[0].m_dynamics.m_velChange == 0);
  REQUIRE(events[1].m_dynamics.m_velChange == 125-1);
  REQUIRE(events[2].m_dynamics.m_velChange == 64-125);
  REQUIRE(events[3].m_dynamics.m_velChange == 48-64);
  REQUIRE(events[4].m_dynamics.m_velChange == 31-48);
  REQUIRE(events[5].m_dynamics.m_velChange == 31-31);
}

