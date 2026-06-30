#pragma once

#include <vector>
#include "LayoutStrategy.h"
#include "TimeValue.h"

// This layout strategy positions glyphs according to their start time
//  within the bar, while minising space used up by the bar.
class LayoutGrid : public LayoutStrategy
{
public:
  LayoutGrid();

  // Calc the width of the note zone of the given bar. 
  float CalcNoteZoneWidth(const Bar& bar) override;

  // Set the positions of the glyphs within the given bar.
  void PositionGlyphs(Bar& bar) override;

  // Public for testing and glyphTimes is also for testing (otherwise we could
  //  directly get these from glyphs in bar).
  static std::vector<TimeValue> ExtractAttackPoints(
    TimeValue barStart, 
    TimeValue barDuration, 
    const std::vector<TimeValue>& glyphTimes
  );

  // Public for testing.
  static float CalculateWeight(
    const std::vector<TimeValue>& attackPoints, 
    float exponent
  );

  // Set tweakable layout params.
  void SetSpacingExponent(float exponent) { m_spacingExponent = exponent; }
  void SetScaleFactor(float scale) { m_scaleFactor = scale; }
  void SetMinNoteZoneWidth(float minWidth) { m_minNoteZoneWidth = minWidth; }

private:
  struct GridLine
  {
    TimeValue time;
    float x;
  };

  float CalcMargin(float noteZoneWidth) const;

  float m_spacingExponent;   
  float m_scaleFactor;       
  float m_minNoteZoneWidth;  
  float m_marginFraction;    
  float m_marginMinFactor;   
};

