#include <iostream>
#include "Anacrusis.h"
#include "TimeVal.h"

namespace MidiScore
{
int GetAnacrusisTicks(std::optional<std::string> anacrusis, int tpq)
{
  int anacrusisTicks = 0;
  if (anacrusis)
  {
     auto timeval = GetTimeValFromString(*anacrusis);
     // Assume anacrusis string is error checked in main() ?
     if (timeval == TimeVal::NONE)
     {
       std::cout << "Bad anacrusis duration.\n";
       return 0;
     }
     // TODO This function doesn't handle dots, it's just a lookup
     anacrusisTicks = CalcTpqMultipleForTimeVal(tpq, timeval);                   }
  return anacrusisTicks;
}
}

