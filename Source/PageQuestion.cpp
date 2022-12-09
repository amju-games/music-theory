// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiText.h>
#include <ReportError.h>
#include "GuiMusicScore.h"
#include "Page.h"
#include "PageQuestion.h"

namespace Amju
{
void PageQuestion::SetPage(Page* page)
{
  m_page = page;
}

void PageQuestionScore::SetUp()
{
  Assert(m_page);

  // Set musical score display from question text
  GuiMusicScore* ms = dynamic_cast<GuiMusicScore*>(
    GetElementByName(m_page->GetGui(), "music-score"));
  Assert(ms);
  // Simple: multiple glyphs. TODO smart ScoreBuilder.
  // Multiple glyphs split by ;
  Strings strs = Split(m_page->GetQuestion()->GetQuestionString(), ';');
  for (const std::string& s : strs)
  {
    if (!ms->AddMultipleGlyphsFromString(s))
    {
      ReportError("Failed to set score glyph: " + s);
      Assert(0);
    }
  }
}

void PageQuestionText::SetUp()
{
  Assert(m_page);
  IGuiText* text = dynamic_cast<IGuiText*>(
    GetElementByName(m_page->GetGui(), "musical-term-text"));
  Assert(text);
  text->SetText(m_page->GetQuestion()->GetQuestionString());
}

}
