#include <cmath>
#include <iostream>
#include <GuiText.h>
#include <Timer.h>
#include "GuiPatch.h"
#include "Hud.h"
#include "HudNumber.h"

namespace Amju
{
static float FractionalPart(float f)
{
  float intpart = 0;
  return std::modf(f, &intpart);
}

static void Flash(GuiElement* elem, bool on)
{
  elem->SetVisible(on);
}

// Keep this out of header file just so changes don't force a big rebuild
struct HudImpl
{
  HudNumber m_playerScore;
  HudNumber m_pointsMultiplier;
  float m_pointsMultTime = 0;
  
  HudNumber m_playerLife;

  // Patch behind player score (numeric points)
  RCPtr<GuiPatch> m_playerScoreBg;
  RCPtr<GuiPatch> m_pointsMultBg;
  PGuiElement m_pointsMultRoot;

  void InitGui(PGuiElement gui, bool reset)
  {
    // Find and store pointers to GUI elements
    m_playerScore.SetGuiElement(gui, "score-text", "score-text-anim-trigger");

    m_pointsMultiplier.SetGuiElement(gui, 
      "points-mult-text", "points-mult-text-anim-trigger");
    m_pointsMultiplier.m_prefix = "x";

    m_playerLife.SetGuiElement(
      gui, "num-lives-text", "num-lives-text-anim-trigger");
    m_playerLife.m_suffix = "%";

    m_playerScoreBg = dynamic_cast<GuiPatch*>(GetElementByName(gui, "score-bg-patch"));
    Assert(m_playerScoreBg);
    m_pointsMultBg = dynamic_cast<GuiPatch*>(GetElementByName(gui, "points-mult-bg-patch"));
    Assert(m_pointsMultBg);
    m_pointsMultRoot = GetElementByName(gui, "player-points-mult-comp");
    Assert(m_pointsMultRoot);

    // Reset score and life values: but only if we are not resuming 
    //  from being paused.
    if (reset)
    {
      m_playerScore.Reset(0);
      m_pointsMultiplier.Reset(1);
      m_playerLife.Reset(100); 
    }
    else 
    {
      m_playerScore.ResumeAfterPause();
      m_pointsMultiplier.ResumeAfterPause();
      m_playerLife.ResumeAfterPause();
    }

    m_pointsMultRoot->SetVisible(m_pointsMultiplier.m_internalNumber > 1);

    SetPatchSizes();
  }

  // Size an element to enclose text in the given text element.
  static void SizePatchToText(
    GuiElement* text, 
    GuiElement* patch, 
    float extra, // add this to the size, for margins on left and right
    bool canShorten = false) 
  {
    Vec2f textSize = dynamic_cast<GuiTextBase*>(text)->CalcSizeToText();
    Vec2f bgSize = patch->GetSize();
    if (canShorten || (textSize.x > (bgSize.x - extra)))
    { 
      bgSize.x = textSize.x + extra;  
    }
    patch->SetSize(bgSize);
  }

  void SetPatchSizes()
  {
    // Set player score bg patch to fit the score
    const float EXTRA_X_SCORE = .1f;
    SizePatchToText(
      m_playerScore.m_guiTextElement, 
      m_playerScoreBg,
      EXTRA_X_SCORE);

    // Set position of points multiplier to the right of the (points) score.
    const float UP_A_BIT = .07f;
    Vec2f scoreBgSize = m_playerScoreBg->GetSize();
    auto pos = m_playerScoreBg->GetCombinedPos() + Vec2f(scoreBgSize.x, UP_A_BIT);
    m_pointsMultRoot->SetLocalPos(pos);

    // Set size of multiplier bg -- it could be large, in theory,
    //  because the multipliers multiply! E.g. x2 -> x5 -> x2 => x20
    const float EXTRA_X_POINTS_MULT = .05f;
    const bool CAN_SHORTEN = true;
    SizePatchToText(
      m_pointsMultiplier.m_guiTextElement, 
      m_pointsMultBg,
      EXTRA_X_POINTS_MULT, CAN_SHORTEN);
  }

  void MultPointsMultiplier(int mult)
  {
    if (m_pointsMultiplier.m_internalNumber  < 1)
    {
      m_pointsMultiplier.m_internalNumber = 1;
    }
    m_pointsMultiplier.m_internalNumber *= mult;

    // Set timer
    const float POINTS_MULT_MAX_TIME = 5.f; // TODO config
    m_pointsMultTime = POINTS_MULT_MAX_TIME;
   
    // Update display
    m_pointsMultiplier.Reset(m_pointsMultiplier.m_internalNumber);

    // Trigger pulse anim 
    m_pointsMultiplier.ResetAnimation();

    // Make sure it's visible!
    m_pointsMultRoot->SetVisible(true);

#ifdef HUD_DEBUG
std::cout << "HUD: Points multiplier is now: " 
  << m_pointsMultiplier.m_internalNumber
  << " (this could be from an earlier Extra Reward).\n";
#endif
  }

  void UpdatePointsMultiplier()
  {
    m_pointsMultiplier.Update();
    if (m_pointsMultiplier.m_internalNumber > 1)
    {
      const float dt = TheTimer::Instance()->GetDt();
      // Count down the time remaining for the multiplier to remain active.
      m_pointsMultTime -= dt;
      if (m_pointsMultTime > 2.f)
      {
      }
      else if (m_pointsMultTime > 1.f)
      {
        // Flash slowly
        // TODO Inverse Hud number
        Flash(m_pointsMultiplier.m_guiTextElement, 
          FractionalPart(m_pointsMultTime * 2.f) < .5f);
      }
      else if (m_pointsMultTime > 0.f)
      {
        // Flash quickly
        Flash(m_pointsMultiplier.m_guiTextElement, 
          FractionalPart(m_pointsMultTime * 4.f) < .5f);
      }
      else
      {
        // Set multiplier to 1; set GUI element to invisible.
        m_pointsMultiplier.Reset(1);
#ifdef HUD_DEBUG
std::cout << "Points multiplier timed out, is now 1.\n";
#endif
      }
    }
    else
    {
      // TODO Hud number visible setting would be handy.
      m_pointsMultRoot->SetVisible(false);
    }
  }

  void Update()
  {
    m_playerScore.Update();

    UpdatePointsMultiplier();

    SetPatchSizes(); // Update sizes of patch bgs for score and multiplier

    m_playerLife.Update();
  }
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
  m_pimpl->InitGui(gui, reset);
}

void Hud::SetPatchSizes()
{
  m_pimpl->SetPatchSizes();
}

void Hud::Update()
{
  m_pimpl->Update();
}

void Hud::MultPointsMultiplier(int mult)
{
  m_pimpl->MultPointsMultiplier(mult);
}

void Hud::AddToPlayerPoints(int add, int frames)
{
  int mult = m_pimpl->m_pointsMultiplier.m_internalNumber;
  int product = add * std::max(mult, 1);
  m_pimpl->m_playerScore.Add(product, frames);

#ifdef HUD_DEBUG
std::cout << "HUD points add: " << add 
  << " mult is: " << mult 
  << " Added points: " << product 
  << "\n";
#endif
}

// Singleton-ish so we can display it across multiple
//  game states.
Hud& GetHud()
{
  static Hud h;
  return h;
}
}

