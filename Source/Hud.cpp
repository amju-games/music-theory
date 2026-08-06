#include <GuiText.h>
#include "GuiPatch.h"
#include "Hud.h"
#include "HudNumber.h"

namespace Amju
{
// Keep this out of header file just so changes don't force a big rebuild
struct HudImpl
{
  HudNumber m_playerScore;
  HudNumber m_pointsMultiplier;

  HudNumber m_playerLife;

  // Patch behind player score (numeric points)
  RCPtr<GuiPatch> m_playerScoreBg;
};

Hud::Hud()
{
  m_pimpl = new HudImpl;
}

Hud::~Hud()
{
  delete m_pimpl;
}

HudNumber& Hud::GetPlayerScore()
{
  return m_pimpl->m_playerScore;
}

HudNumber& Hud::GetPlayerLife()
{
  return m_pimpl->m_playerLife;
}

void Hud::InitGui(PGuiElement gui, bool reset)
{
  // Find and store pointers to GUI elements
  m_pimpl->m_playerScore.SetGuiElement(gui, "score-text", "score-text-anim-trigger");

  m_pimpl->m_pointsMultiplier.SetGuiElement(gui, 
    "points-mult-text", "points-mult-text-anim-trigger");
  m_pimpl->m_pointsMultiplier.m_prefix = "x";

  m_pimpl->m_playerLife.SetGuiElement(
    gui, "num-lives-text", "num-lives-text-anim-trigger");
  m_pimpl->m_playerLife.m_suffix = "%";

  // Reset score and life values: but only if we are not resuming 
  //  from being paused.
  if (reset)
  {
    m_pimpl->m_playerScore.Reset(0);
    m_pimpl->m_pointsMultiplier.Reset(0);
    m_pimpl->m_playerLife.Reset(100); 
  }
  else 
  {
    m_pimpl->m_playerScore.ResumeAfterPause();
    m_pimpl->m_pointsMultiplier.ResumeAfterPause();
    m_pimpl->m_playerLife.ResumeAfterPause();
  }

  m_pimpl->m_playerScoreBg = dynamic_cast<GuiPatch*>(GetElementByName(gui, "score-bg-patch"));

  SetPatchSizes();
}

void Hud::SetPatchSizes()
{
  Vec2f textSize = dynamic_cast<GuiTextBase*>(
    m_pimpl->m_playerScore.m_guiTextElement.GetPtr())->CalcSizeToText();

  Vec2f bgSize = m_pimpl->m_playerScoreBg->GetSize();
  const float EXTRA_X = .1f;
  if (textSize.x > (bgSize.x - EXTRA_X))
  { 
    bgSize.x = textSize.x + EXTRA_X;  
  }
  m_pimpl->m_playerScoreBg->SetSize(bgSize);
}

void Hud::Update()
{
  m_pimpl->m_playerScore.Update();
  m_pimpl->m_pointsMultiplier.Update();
  
  SetPatchSizes();

  m_pimpl->m_playerLife.Update();
}

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud()
{
  static Hud h;
  return h;
}
}

