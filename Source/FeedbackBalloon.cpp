#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiText.h>
#include <Localise.h>
#include "FeedbackBalloon.h"
#include "Grader.h"
#include "Palette.h"

namespace Amju
{
static const Colour& GetColourForGrade(float f)
{
  static Palette palette;
  [[maybe_unused]] static bool doOnce = [&]()
  {
    const char* PALETTE_NAME = "Image/palette-balloon.png";

    return palette.Load(PALETTE_NAME);
  } ();
  return palette.GetColour(f); 
}

void SetFeedbackBalloonVisible(bool showNotHide, PGuiElement gui)
{
  auto elem = GetElementByName(gui, "feedback-balloon");
  Assert(elem);
  elem->SetVisible(showNotHide);
}

void SetUpFeedbackBalloon(const Grade& g, PGuiElement gui)
{
  Assert(g.m_type != Grade::UNGRADED);

  SetFeedbackBalloonVisible(true, gui);

  // Set text inside balloon
  Vec2f textSize;
  auto elem = GetElementByName(gui, "feedback-text");
  Assert(elem); // feedback balloon gui error?!
  auto t = dynamic_cast<GuiTextBase*>(elem);
  Assert(t);
  t->SetText(Lookup(g.m_feedback));
  textSize = t->CalcSizeToText();

  // Set size of background images to fit text, plus a margin.
  const float MARGIN = 0.1f;
  elem = GetElementByName(gui, "bg1"); 
  Assert(elem);
  Vec2f bgSize = elem->GetSize();
  bgSize.x = textSize.x + MARGIN;
  elem->SetSize(bgSize);
  elem = GetElementByName(gui, "bg2"); 
  Assert(elem);
  elem->SetSize(bgSize); // same size initially but grow at different rates.

  // Set balloon colour: lookup from palette.
  // (We can't use GuiDecColourPalette here without doing some surgery
  //  elsewhere: we would need to set the const value of the controlling
  //  Animation. I don't think that's easily to hand atm.)
  elem = GetElementByName(gui, "colour-balloon");
  Assert(elem);
  auto c = dynamic_cast<GuiDecColour*>(elem);
  Assert(c);
  c->SetColour(GetColourForGrade(g.m_score));
  // Also set second colour, as we will interpolate.. but that could be nice!
  c->SetSecondColour(GetColourForGrade(1.f - g.m_score)); // TODO TEMP TEST

  // Reset animation
  elem = GetElementByName(gui, "moving-anim");
  Assert(elem);
  auto a = dynamic_cast<GuiDecAnimation*>(elem);
  Assert(a);
  a->ResetAnimation();
}
}

