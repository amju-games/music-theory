#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Bar.h"
#include "LayoutGrid.h"

// A safe threshold well below a 64th note triplet (TIMEVAL_QQQ is 0.0625f)
static const float TIME_EPSILON = 0.0005f;

LayoutGrid::LayoutGrid()
  : m_spacingExponent(0.5f)
  , m_scaleFactor(1.5f)          
  , m_minNoteZoneWidth(3.0f)
  , m_marginFraction(1.0f / 20.0f)
  , m_marginMinFactor(1.5f)
{
}

float LayoutGrid::CalcMargin(float noteZoneWidth) const
{
  float m = noteZoneWidth * m_marginFraction; 
  return std::max(m, NOTE_HEAD_WIDTH * m_marginMinFactor); 
}

std::vector<TimeValue> LayoutGrid::ExtractAttackPoints(
  TimeValue barStart, 
  TimeValue barDuration, 
  const std::vector<TimeValue>& glyphTimes
)
{
  std::vector<TimeValue> points;
  points.reserve(glyphTimes.size() + 2);
  
  points.push_back(barStart);
  for (TimeValue t : glyphTimes)
  {
    points.push_back(t);
  }
  points.push_back(barStart + barDuration);

  // 1. Sort the float times chronologically
  std::sort(points.begin(), points.end());

  // 2. Wipe out duplicates using an epsilon-based lambda predicate
  auto lastUnique = std::unique(points.begin(), points.end(), [](TimeValue a, TimeValue b) {
    return std::abs(a - b) < TIME_EPSILON;
  });
  points.erase(lastUnique, points.end());

#ifdef DEBUG_ATTACK_POINTS
std::cout << "// Bar start: " << barStart << " Extracted attack points: ";
for (auto p : points) std::cout << p << " ";
std::cout << "\n";
#endif

  return points;
}

float LayoutGrid::CalculateWeight(const std::vector<TimeValue>& attackPoints, float exponent)
{
  if (attackPoints.size() < 2)
  {
    return 0.0f;
  }

  float totalWeight = 0.0f;
  for (size_t i = 0; i < attackPoints.size() - 1; ++i)
  {
    float duration = attackPoints[i + 1] - attackPoints[i];
    if (duration > TIME_EPSILON)
    {
      totalWeight += std::pow(duration, exponent);
    }
  }
  return totalWeight;
}

float LayoutGrid::CalcNoteZoneWidth(const Bar& bar) 
{
  std::vector<TimeValue> glyphTimes;
  for (const auto& g : bar.GetGlyphs())
  {
    glyphTimes.push_back(g->GetTimes().GetStartTimeValue());
  }

  auto attackPoints = ExtractAttackPoints(bar.GetStartTime(), bar.GetDuration(), glyphTimes);
  float barWeight = CalculateWeight(attackPoints, m_spacingExponent);

  float noteZoneWidth = barWeight * m_scaleFactor;

#ifdef DEBUG_CALC_NOTE_ZONE_WIDTH
std::cout << "// CalcNoteZoneWidth: barWeight: " << barWeight 
  << " noteZoneWidth: " << noteZoneWidth
  << " final value: " << std::max(noteZoneWidth, m_minNoteZoneWidth)
  << "\n";
#endif

  return std::max(noteZoneWidth, m_minNoteZoneWidth);
}

void LayoutGrid::PositionGlyphs(Bar& bar) 
{
  float noteZoneWidth = bar.GetNoteZoneWidth();
  
  if (noteZoneWidth <= 0.0f)
  {
    assert(0);
    noteZoneWidth = CalcNoteZoneWidth(bar);
  }

  const float margin = CalcMargin(noteZoneWidth);
  float printableWidth = noteZoneWidth - (2.0f * margin);
  if (printableWidth <= 0.0f)
  {
    return;
  }

  // 1. Gather our unique attack points
  std::vector<TimeValue> glyphTimes;
  for (const auto& g : bar.GetGlyphs())
  {
    glyphTimes.push_back(g->GetTimes().GetStartTimeValue());
  }
  auto attackPoints = ExtractAttackPoints(bar.GetStartTime(), bar.GetDuration(), glyphTimes);
  
  // 2. CRIMINAL PREVENTION: Find the structural padding required for each attack point.
  // We map each unique TimeValue to the maximum extra width needed by its glyphs (e.g., accidentals).
  std::map<TimeValue, float> ColumnPadding;
  for (TimeValue t : attackPoints)
  {
    ColumnPadding[t] = 0.0f;
  }

  auto& glyphs = bar.GetGlyphs();
  for (auto& g : glyphs)
  {
    TimeValue glyphTime = g->GetTimes().GetStartTimeValue();
    
    // Find which unique attack point grid line this glyph snaps to
    for (TimeValue t : attackPoints)
    {
      if (std::abs(t - glyphTime) < TIME_EPSILON)
      {
        // GetTotalWidth includes the notehead + accidental widths.
        // The extra space needed to the left of the nominal note center:
        float extraLeftSpace = GetTotalWidth(*g) - NOTE_HEAD_WIDTH;
        
        if (extraLeftSpace > ColumnPadding[t])
        {
          ColumnPadding[t] = extraLeftSpace;
        }
        break;
      }
    }
  }

  // 3. Subtract all required accidental paddings from our total printable width 
  // so the non-linear rhythmic scaling rules apply only to the remaining "elastic" space.
  float totalPaddingRequired = 0.0f;
  for (const auto& pair : ColumnPadding)
  {
    totalPaddingRequired += pair.second;
  }

  float elasticWidth = printableWidth - totalPaddingRequired;
  
  // Fallback fallback: If accidentals completely choke the bar, use a tight layout
  if (elasticWidth < 0.0f)
  {
    elasticWidth = 0.0f;
  }

  float totalWeight = CalculateWeight(attackPoints, m_spacingExponent);
  if (totalWeight <= 0.0f)
  {
    return;
  }

  // 4. Build the grid line coordinates, injecting the padding right before the step
  std::vector<GridLine> gridPositions;
  gridPositions.reserve(attackPoints.size());
  
  float currentX = margin;

  for (size_t i = 0; i < attackPoints.size() - 1; ++i)
  {
    TimeValue currentTime = attackPoints[i];
    
    // Inject the physical padding required for this specific column's accidentals
    currentX += ColumnPadding[currentTime];
    gridPositions.push_back({ currentTime, currentX });

    // Distribute the elastic space proportionally based on time duration
    float duration = attackPoints[i + 1] - attackPoints[i];
    float stepWeight = std::pow(duration, m_spacingExponent);
    float stepWidth = (stepWeight / totalWeight) * elasticWidth;
    currentX += stepWidth;
  }
  
  // Add the final boundary line anchor
  gridPositions.push_back({ attackPoints.back(), currentX + ColumnPadding[attackPoints.back()] });

  // 5. Position the glyphs safely using the padded grid coordinates
  const float xfudge = -NOTE_HEAD_WIDTH * 0.5f; 
  const float noteZoneLeftX = bar.GetNoteZoneLeftX();

  for (auto& g : glyphs)
  {
    TimeValue glyphTime = g->GetTimes().GetStartTimeValue();
    float xPosInBar = margin;

    for (const auto& line : gridPositions)
    {
      if (std::abs(line.time - glyphTime) < TIME_EPSILON)
      {
        xPosInBar = line.x;
        break;
      }
    }

    g->SetPos(xPosInBar + xfudge + noteZoneLeftX, g->GetY() + bar.GetY()); 
  }
}

