// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <GuiDecAnimation.h>
#include <GuiText.h>
#include <ReportError.h>
#include "GSPages.h"
#include "GuiMusicScore.h"
#include "Page.h"
#include "PageQuestion.h"

namespace Amju
{
struct PlayCommand : public GuiCommand
{
  PlayCommand(GuiElement* guiRoot) : m_guiRoot(guiRoot) {}

  bool Do() override
  {
std::cout << "Play button command: root node is: " << m_guiRoot->GetName() << "\n";

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
    ////button->SetIsFocusButton(false);

    // Set callback to re-enable the play button when anim completes.
    // Animation node should be child of anim.
    anim = dynamic_cast<GuiDecAnimation*>(GetElementByName(anim, "play-music-anim"));
    if (anim)
    {
      anim->SetOnCompleteCallback([button](Animator*) 
      { 
        button->SetIsEnabled(true); 
        ////button->SetIsFocusButton(true); 
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
  PGuiElement guiRoot = TheGSPages::Instance()->GetGui();
  GuiButton* playButton = dynamic_cast<GuiButton*>(
    GetElementByName(guiRoot, "play-button"));

  if (playButton)
  {
std::cout << "Found play button\n";
    if (ms->HasAnimation())
    {
std::cout << "Score has animation, so setting play button command\n";
      playButton->SetVisible(true);
      playButton->SetIsEnabled(true);
      //playButton->SetHasFocus(true);
      playButton->SetCommand(new PlayCommand(m_page->GetGui()));
    }
    else
    {
std::cout << "Score has no animation, so hiding play button\n";
      playButton->SetVisible(false);
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
    GetElementByName(TheGSPages::Instance()->GetGui(), "play-button"));
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
