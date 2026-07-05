#include <GuiDecAnimation.h>
#include <GuiText.h>
#include "FeedbackBalloon.h"
#include "Grader.h"

namespace Amju
{
void SetFeedbackBalloonVisible(bool showNotHide, PGuiElement gui)
{
  auto elem = GetElementByName(gui, "feedback-balloon");
  if (elem)
  {
    elem->SetVisible(showNotHide);
  }
}

void SetUpFeedbackBalloon(const Grade& g, PGuiElement gui)
{
  Assert(g.m_type != Grade::UNGRADED);

  if (g.m_score > 0.5f)
  {
    // Sounds terrible //PlayWav("good1");
  }
  else 
  {
    //PlayWav("rubber_ducky");
  }

  SetFeedbackBalloonVisible(true, gui);

  auto elem = GetElementByName(gui, "feedback-text");
  if (elem)
  {
    auto t = dynamic_cast<IGuiText*>(elem);
    if (t)
    {    
      t->SetText(g.m_feedback);
    }    
  }

  // Reset animation
  elem = GetElementByName(gui, "moving-anim");
  if (elem)
  {
    auto a = dynamic_cast<GuiDecAnimation*>(elem);
    if (a)
    {
      a->ResetAnimation();
    }
  }

  // TODO Set balloon colours according to score
}
}

