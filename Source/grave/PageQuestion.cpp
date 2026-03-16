// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>
#include <ReportError.h>
#include "GSPages.h"
#include "GuiMusicScore.h"
#include "Page.h"
#include "PageQuestion.h"
#include "UserProfile.h"

namespace Amju
{
struct PlayCommand : public GuiCommand
{
  PlayCommand(GuiElement* guiRoot) : m_guiRoot(guiRoot) {}

  bool Do() override
  {
    auto profile = TheUserProfile();
    int numHints = profile->GetHints(HintType::HINT_TYPE_PLAY);
    Assert(numHints > 0); // Button should be disabled if hints < 1
    profile->AddHints(HintType::HINT_TYPE_PLAY, -1);
    // Update the HUD
    GetPagesState()->ShowHints();

    GuiDecAnimation* anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(
      m_guiRoot, "play-music-trigger"));
    if (!anim)
    {
      return false;
    }
    anim->ResetAnimation();
    anim->SetEaseType(GuiDecAnimation::EaseType::EASE_TYPE_ONE);

    // Disable play button so we can't spam it
    GuiButton* button = dynamic_cast<GuiButton*>(GetGuiElement());
    Assert(button);
    button->SetIsEnabled(false);

    // Set callback to re-enable the play button when anim completes.
    // Animation node should be child of anim.
    anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(anim, "play-music-anim"));
    if (anim)
    {
      anim->SetOnCompleteCallback([button](Animator*) 
      { 
        button->SetIsEnabled(true); 
      });
    }
    return false; // Can't undo
  }

  PGuiElement m_guiRoot;
}; 

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
  PGuiElement guiRoot = GetPagesState()->GetGui();
  GuiButton* playButton = dynamic_cast<GuiButton*>(
    GetElementByName(guiRoot, "play-button"));

  if (playButton)
  {
std::cout << "Found play button\n";
    if (ms->HasAnimation())
    {
      // Check we have hints available
      int hints = TheUserProfile()->GetHints(HintType::HINT_TYPE_PLAY);

std::cout << "Score has animation, so setting play button command\n";
      playButton->SetVisible(true);
      playButton->SetIsEnabled(hints > 0);
      //playButton->SetHasFocus(true);
      playButton->SetCommand(new PlayCommand(m_page->GetGui()));
    }
    else
    {
std::cout << "Score has no animation, so hiding play button\n";
      playButton->SetIsEnabled(false); //SetVisible(false);
    }
  }
  else
  {
std::cout << "No play button found.\n";
  }
}

void PageQuestionScore::OnPlayerChoice()
{
  // Disable Play button once player has answered
  GuiButton* playButton = dynamic_cast<GuiButton*>(
    GetElementByName(GetPagesState()->GetGui(), "play-button"));
  if (playButton)
  {
    playButton->SetIsEnabled(false);
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
