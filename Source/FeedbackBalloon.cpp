#include <GuiDecAnimation.h>
#include <GuiText.h>
#include "FeedbackBalloon.h"
#include "Grader.h"

namespace Amju
{
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
  t->SetText(g.m_feedback);
  textSize = t->CalcSizeToText();

  // Set size of background images to fit text, plus a margin.
  const float MARGIN = 0.1f;
  elem = GetElementByName(gui, "bg1"); // TODO name
  Assert(elem);
  Vec2f bgSize = elem->GetSize();
  bgSize.x = textSize.x + MARGIN;
  elem->SetSize(bgSize);
  elem = GetElementByName(gui, "bg2"); // TODO name
  Assert(elem);
  elem->SetSize(bgSize); // same size initially but grow at different rates.

  // Reset animation
  elem = GetElementByName(gui, "moving-anim");
  Assert(elem);
  auto a = dynamic_cast<GuiDecAnimation*>(elem);
  Assert(a);
  a->ResetAnimation();

  // TODO Set balloon colours according to score
}
}

