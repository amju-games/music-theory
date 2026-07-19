#pragma once

#include "Event.h"

namespace MidiScore
{
// * Quantiser *
// Interface for different quantisation strategies.
class Quantiser
{
public:
  Quantiser(TimeVal res = TimeVal::NONE) : m_res(res) {}

  virtual ~Quantiser() = default;

  // Quantise start time of one event
  virtual void QuantiseStartTime(int tpq, Event& e) const = 0;

  // Quantise duration of one event
  virtual void QuantiseDuration(int tpq, Event& e) const = 0;

  // Second pass over all events
  virtual void SecondPass(Events& events) const = 0;

protected:
  TimeVal m_res;
};

// * MakeQuantiser *
// Create instance of Quantiser sub type according to params 
//  (TODO add more params as required).
// Return ref to the instance, (which will persist on the heap until 
//  process end).
Quantiser& MakeQuantiser(TimeVal res);

// * MonoQuantiser *
// Quantises for a monophonic instrument; i.e. no overlapping notes.
class MonoQuantiser : public Quantiser
{
public:
  MonoQuantiser(TimeVal res) : Quantiser(res) {}

  void QuantiseStartTime(int tpq, Event& e) const override;
  void QuantiseDuration(int tpq, Event& e) const override;
  void SecondPass(Events& events) const override;
};

// * NullQuantiser *
// For when we don't need to quantise at all.
class NullQuantiser : public Quantiser
{
public:
  NullQuantiser() = default;

  void QuantiseStartTime([[maybe_unused]]int tpq, [[maybe_unused]] Event& e) const override {}
  void QuantiseDuration([[maybe_unused]] int tpq, [[maybe_unused]] Event& e) const override {}
  void SecondPass([[maybe_unused]] Events& events) const override {}
};
}

