#include <iostream>
#include "Bar.h"
#include "LayoutFull.h"

float LayoutFull::CalcNoteZoneWidth(const Bar& bar) 
{
  // In this layout strategy, we position notes based fully on their
  //  start time in the bar. So the width of the note zone is proportional
  //  to the number of beats. 

  // This calculation gives us a relative width for this bar's note zone.
  // It isn't the final width, so we can't cache it for later.
  float noteZoneWidth = static_cast<float>(bar.GetNumBeats());

  // Scale the note zone width so it's using the same notional units as
  //  the pre note zone width. This depends on how the note zone width is
  //  being generated.
  noteZoneWidth *= .5f;

  // TODO Add more space for accidentals.

  return noteZoneWidth;
}

static float CalcMargin(float noteZoneWidth)
{
  // Margin between start of note zone and first note/rest.

  // TODO One day bring out all these hidden constants into a config file,
  //  or input params.

  // Set margin as a fraction of the overall note zone width.
  const float MARGIN_FRACTION_OF_TOTAL_WIDTH = 1.f / 20.f;
  float m = noteZoneWidth * MARGIN_FRACTION_OF_TOTAL_WIDTH; 

  // Set minimum margin width
  const float MARGIN_MIN = 1.5f;
  m = std::max(m, NOTE_HEAD_WIDTH * MARGIN_MIN); 

  // TODO max margin width

  return m;
}

static float CalcBeatWidth(int numBeats, float noteZoneWidth)
{
  // Divide remaining width by the number of beats to get 
  //  the distance between each beat.
  if (numBeats > 1)
  {
    return noteZoneWidth / numBeats;
  }
  else
  {
    return 1.f;
  }
}

void LayoutFull::PositionGlyphs(Bar& bar) 
{
  // Get each glyph. Set its x-coord directly proportional to its start
  //  time in the bar.

  float noteZoneWidth = bar.GetNoteZoneWidth();

  // Reduce the note zone width by a margin on either side.
  // margin is distance from leftX to first glyph.
  // The distance from last glyph to right bar line will be more, because the
  //  final note won't be on the final beat - that beat is the first beat
  //  of the next bar.
  const float margin = CalcMargin(noteZoneWidth);

  // Reduce the note zone width by the margin at left and right.
  noteZoneWidth -= 2.f * margin;
  
  // Distance between beats in this bar
  const int numBeats = bar.GetNumBeats();
  const float beatWidth = CalcBeatWidth(numBeats, noteZoneWidth);

  // Compensate for glyph width, move to the left a bit
  // TODO depends on glyph type?, e.g. semibreve is slightly wider.
  const float xfudge = -NOTE_HEAD_WIDTH * .5f; 

  const float noteZoneLeftX = bar.GetNoteZoneLeftX();

  // Set the x-coord of each glyph in the note zone.
  auto& glyphs = bar.GetGlyphs();
  for (auto& g : glyphs)
  {
    // Get the number of beats into the bar where this glyph lives.
    TimeValue glyphTimeInBar = 
      g->GetTimes().GetStartTimeValue() - bar.GetStartTime();

    // Mult beat position by width of one beat to get x-coord.
    float xPosInBar = beatWidth * glyphTimeInBar + margin + xfudge;

    // We are using the current glyph y-coord and adding to itself --
    //  would it be better to set y-coords separately?
    g->SetPos(xPosInBar + noteZoneLeftX, g->GetY() + bar.GetY()); 
  }
}

