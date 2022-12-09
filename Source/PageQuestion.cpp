// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>
#include <ReportError.h>
#include "GuiMusicScore.h"
#include "Page.h"
#include "PageQuestion.h"

namespace Amju
{
namespace
{
void OnPlayButton(GuiElement* elem)
{
  // Trigger animation, by setting ease type to 'one'
  // First, find the anim
  GuiElement* root = elem->GetParent();
  if (!root)
  {
    return;
  }

  GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(root, "play-music-trigger"));
  if (!anim)
  {
    return;
  }
  anim->ResetAnimation();
  anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);
}

} // anon namespace

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
  
  // Hook up play button -- hide if nothing to play
  GuiButton* playButton = dynamic_cast<GuiButton*>(
    GetElementByName(m_page->GetGui(), "play-button"));
  Assert(playButton);
  if (ms->HasAnimation())
  {
    playButton->SetHasFocus(true);
    playButton->SetCommand(OnPlayButton);
  }
  else
  {
    playButton->SetVisible(false);
  }
}

void PageQuestionScore::OnPlayerChoice()
{
  // Hide Play button once player has answered
  GuiButton* playButton = dynamic_cast<GuiButton*>(
    GetElementByName(m_page->GetGui(), "play-button"));
  Assert(playButton);
  playButton->SetVisible(false);
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
