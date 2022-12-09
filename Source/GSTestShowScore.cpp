// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include "GSTestShowScore.h"
#include "GuiMusicScore.h"

namespace Amju
{
GSTestShowScore::GSTestShowScore()
{
  m_guiFilename = "Gui/gs_test_show_score.txt";
}

void GSTestShowScore::OnActive()
{
  GSBase::OnActive();

  // Set times on glyphs
  GuiMusicScore* ms = dynamic_cast<GuiMusicScore*>(GetElementByName(m_gui, "music-score"));
  int n = ms->GetNumGlyphs();
  for (int i = 0; i < n; i++)
  {
    GuiMusicScore::Glyph& g = ms->GetGlyph(i);
    float t = static_cast<float>(i) / static_cast<float>(n);
    g.SetTimeMinMax(Vec2f(t, t + 0.2f));
  }
}
}
