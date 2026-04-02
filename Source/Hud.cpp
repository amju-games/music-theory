#include "Hud.h"

namespace Amju
{
void Hud::InitGui(PGuiElement gui, bool reset)
{
  // Find and store pointers to GUI elements
  m_playerScore.SetGuiElement(gui, "score-text", "score-text-anim-trigger");
  m_playerLife.SetGuiElement(
    gui, "num-lives-text", "num-lives-text-anim-trigger");

  // Reset score and life values: but only if we are not resuming 
  //  from being paused.
  if (reset)
  {
    m_playerScore.Reset(0);
    m_playerLife.Reset(100); 
  }
  else 
  {
    m_playerScore.ResumeAfterPause();
    m_playerLife.ResumeAfterPause();
  }

  m_playerScoreBg = dynamic_cast<GuiPatch*>(GetElementByName(gui, "score-bg-patch"));

  SetPatchSizes();
}

void Hud::SetPatchSizes()
{
  auto bgSize = m_playerScoreBg->GetSize();
  auto numSize = m_playerScore.m_guiTextElement->GetSize(); 
  bgSize.x = numSize.x; 
  m_playerScoreBg->SetSize(bgSize);
}

void Hud::Update()
{
  m_playerScore.Update();
  m_playerLife.Update();
}

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud()
{
  static Hud h;
  return h;
}
}

