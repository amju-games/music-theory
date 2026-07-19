#include <cassert>
#include <cmath>
#include "Quantiser.h"

namespace MidiScore
{
Quantiser& MakeQuantiser(TimeVal res)
{
  static Quantiser* q = new MonoQuantiser(res);
  return *q;
}

void MonoQuantiser::QuantiseStartTime(int tpq, Event& e) const 
{
  // If -1, unquantised start time was not set when
  //  event was created.
  assert(e.m_unquantisedStart > -1);

  int mult = CalcTpqMultipleForTimeVal(tpq, m_res);

  // Get closest multiple of mult to the unquantised start time.
  e.m_start = mult * static_cast<int>(std::round(
    static_cast<float>(e.m_unquantisedStart) /
    static_cast<float>(mult)));
}

void MonoQuantiser::QuantiseDuration(int tpq, Event& e) const 
{
  assert(e.m_unquantisedDuration > -1);

  int mult = CalcTpqMultipleForTimeVal(tpq, m_res);

  // Here we ensure that duration is at least the quant resolution --
  //  otherwise the note would disappear, and surely we don't want 
  //  that, right???
  e.m_duration = mult * static_cast<int>(std::max(1.f,  // duration is at least resolution
    std::round(
      static_cast<float>(e.m_unquantisedDuration) / 
      static_cast<float>(mult))));
 
  // DON'T set time val! First we need to consider splitting the
  //  note into two or more tied notes.
  //e.SetTimeVal(tpq); 
}

void MonoQuantiser::SecondPass([[maybe_unused]] Events& events) const
{
}
}

