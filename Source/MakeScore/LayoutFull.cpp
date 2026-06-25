#include "Bar.h"
#include "LayoutFull.h"

float LayoutFull::CalcNoteZoneWidth(const Bar& bar) 
{
  // In this layout strategy, we position notes based fully on their
  //  start time in the bar. So the width of the note zone is proportional
  //  to the number of beats. 
  // TODO Add a scale factor: actually, this could go in the calling code,
  //  so we can always scale the pre-note/note zone relative width..?
  float w = static_cast<float>(bar.GetNumBeats());
  return w;
}

static float CalcMargin(float noteZoneWidth, int numBeats)
{
  float m = noteZoneWidth / (numBeats + 2.0f);
  m = std::max(m, NOTE_HEAD_WIDTH * 1.5f); // minimum margin width
  // TODO max margin width
  return m;
}

static float CalcBeatWidth(int numBeats, float noteZoneWidth)
{
  // Divide remaining width by the number of beats to get 
  //  the distance between each beat.
  if (numBeats > 1)
  {
    return noteZoneWidth / (numBeats - 1.0f);
  }
  else
  {
    return noteZoneWidth;
  }
}

void LayoutFull::PositionGlyphs(Bar& bar) 
{
  // Get each glyph. Set its x-coord directly proportional to its start
  //  time in the bar.

  const int numBeats = bar.GetNumBeats();

  // TODO Refactor, this is terrible
  const auto [_, preNoteZoneWidth] = bar.PreNoteZoneToString();
  float noteZoneWidth = bar.GetWidth() - preNoteZoneWidth;
  const float noteZoneStart = bar.GetX() + preNoteZoneWidth;

  // margin is distance from leftX to first glyph, and also distance
  //  from last glyph to right bar line.
  const float margin = CalcMargin(noteZoneWidth, numBeats);

  // Reduce the remaining bar width by the margin at left and right.
  noteZoneWidth -= 2 * margin;
  
  // Distance between beats in this bar
  const float beatWidth = CalcBeatWidth(numBeats, noteZoneWidth);

  // Compensate for glyph width, move to the left a bit
  // TODO Get rid of this!
  // TODO depends on glyph type?, e.g. semibreve is slightly wider.
  float xfudge = -0.2f;

  const auto& glyphs = bar.GetGlyphs();
  for (auto& g : glyphs)
  {
    // Get the number of beats into the bar where this glyph lives
    TimeValue glyphTimeInBar = 
      g->GetTimes().GetStartTimeValue() - bar.GetStartTime();

    // Mult beat position by width of one beat to get "final" pos in bar
    float xPosInBar = beatWidth * glyphTimeInBar + margin + xfudge;
    g->SetPos(xPosInBar + noteZoneStart, g->GetY() + bar.GetY()); 
  }

}

