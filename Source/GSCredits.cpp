#include <GuiDecTranslate.h>
#include <GuiText.h>
#include <Timer.h>
#include "GSCredits.h"

namespace Amju
{
static void OnBackButton(GuiElement* elem)
{
  TheGSCredits::Instance()->GoBack();
}

// This is a convenience function for OTHER states, not called in here!
void OnCreditsButton(GuiElement* elem)
{
  // TODO Play button song
  //PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music

  TheGSCredits::Instance()->SetPrevState(TheGame::Instance()->GetState());
  TheMessageQueue::Instance()->Clear(); // kill timeline events
  GoTo<TheGSCredits>();
}

GSCredits::GSCredits()
{
  m_guiFilename = "Gui/gs_credits.txt";
  m_sceneFilename = "Scene/credits-scene.txt";
  // Constants for non-auto-test mode
  const float WAIT_TIME = 3.f;
  const float SCROLL_VEL = .5f; // screen units/sec
  m_waitTime = WAIT_TIME; 
  m_scrollVel = SCROLL_VEL;
}

void GSCredits::OnActive()
{
  GSBase3d::OnActive(); // Does not automatically load 3d scene.
  Reload3d();

  // Start playing music  -- timeline event

  // Set up back button
  // TODO

  // Quit button (red, top right)
  auto backButton = GetElementByName(m_gui, "quit-button");
  backButton->SetCommand(OnBackButton);
//  backButton->SetHasFocus(true);

  SetVersionText();
}

void GSCredits::AutoTestSetup()
{
  const float WAIT_TIME = 0.3f;
  const float SCROLL_VEL = 5.f; // screen units/sec
  m_waitTime = WAIT_TIME; 
  m_scrollVel = SCROLL_VEL;
}

void GSCredits::Update()
{
  GSBase3d::Update();

  // Scroll the logo and all text.
  if (GetTimeInThisState() > m_waitTime)
  {
    auto translate = dynamic_cast<GuiDecTranslate*>(
      m_gui->GetElementByName("translate-text-and-logo"));
    Assert(translate);
    auto pos = translate->GetLocalPos();
    pos.y += m_scrollVel * TheTimer::Instance()->GetDt();
    translate->SetLocalPos(pos);
  }

  // Check for the bottom of the credits text reaching the top of the screen.
  Assert(m_gui);
  auto text = dynamic_cast<GuiTextBase*>(
    m_gui->GetElementByName("credits-text"));
  Assert(text);
  float y = text->GetCombinedPos().y - text->CalcSizeToText().y;
  if (y > 1.f)
  {
    Amju::OnBackButton(nullptr);
  }
}
}

