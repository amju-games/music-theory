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
  Dynamics::SetLastDynamicsString(); // reset duplicate remover

  REQUIRE(events[0].m_dynamics.m_velocity == 1);
  REQUIRE(events[1].m_dynamics.m_velocity == 125); // ok, setting vel works

  REQUIRE(events[0].m_dynamics.ToString() == "ppp");
  REQUIRE(events[1].m_dynamics.ToString() == "fff");
  REQUIRE(events[2].m_dynamics.ToString() == ""); // no mark required until we get to events[4] 
  REQUIRE(events[3].m_dynamics.ToString() == "");
  REQUIRE(events[4].m_dynamics.ToString() == "pp");
  REQUIRE(events[5].m_dynamics.ToString() == "");
}

TEST_CASE("Dynamics processing: final note", "[Dynamics]")
{
  Events events;
  AppendNoteEventToEvents(n(60, 1), events); // ppp
  AppendNoteEventToEvents(n(62, 1), events); // ppp
  AppendNoteEventToEvents(n(62, 1), events); // ppp
  AppendNoteEventToEvents(n(64, 64), events); // mf

  InsertDynamics(events);
  Dynamics::SetLastDynamicsString(); // reset duplicate remover

  REQUIRE(events[0].m_dynamics.ToString() == "ppp");
  REQUIRE(events[1].m_dynamics.ToString() == "");

  // This next event is set to "ppp" by the algo but stripped out
  //  in Dynamics::ToString, as we want to remove duplicates.
  REQUIRE(events[2].m_dynamics.ToString() == "");

  REQUIRE(events[3].m_dynamics.ToString() == "mf");
}

TEST_CASE("Dynamics processing: all the same", "[Dynamics]")
{
  Events events;
  AppendNoteEventToEvents(n(60, 75), events); // mf
  AppendNoteEventToEvents(n(62, 75), events); // mf
  AppendNoteEventToEvents(n(64, 75), events); // mf
  AppendNoteEventToEvents(n(65, 75), events); // mf

  InsertDynamics(events);
  Dynamics::SetLastDynamicsString(); // reset duplicate remover

  REQUIRE(events[0].m_dynamics.ToString() == "mf");
  REQUIRE(events[1].m_dynamics.ToString() == "");
  REQUIRE(events[2].m_dynamics.ToString() == "");
  REQUIRE(events[3].m_dynamics.ToString() == "");
}

TEST_CASE("Dynamics processing: down and up", "[Dynamics]")
{
  Events events;
  AppendNoteEventToEvents(n(60, 75), events); // mf
  AppendNoteEventToEvents(n(62, 50), events); // mp
  AppendNoteEventToEvents(n(64, 25), events); // pp
  AppendNoteEventToEvents(n(65, 50), events); // mp
  AppendNoteEventToEvents(n(67, 75), events); // mf

  InsertDynamics(events);
  Dynamics::SetLastDynamicsString(); // reset duplicate remover

  REQUIRE(events[0].m_dynamics.ToString() == "mf");
  REQUIRE(events[1].m_dynamics.ToString() == "");
  REQUIRE(events[2].m_dynamics.ToString() == "pp"); 
  REQUIRE(events[3].m_dynamics.ToString() == "");
  REQUIRE(events[4].m_dynamics.ToString() == "mf");
}

