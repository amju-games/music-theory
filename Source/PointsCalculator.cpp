#include <cmath>
#include <iostream>
#include "Grader.h"
#include "PointsCalculator.h"

namespace Amju
{
int CalcPoints(const Grade& grade)
{
  int amount = static_cast<int>(std::round(grade.m_score * 10.f));
  amount *= 10;

  // TODO If there is a points multiplier on the note, we should apply
  //  it to this note. 
  // The points multiplier value isn't immediately set, not before 
  //  we reach this function.

#ifdef CALC_POINTS_DEBUG
std::cout << "Points calc: grade score: " << grade.m_score 
  << " points: " << amount
  << "\n";
#endif
 
  return amount;
};
}

