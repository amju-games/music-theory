#pragma once

#include "GuiElement.h"

namespace Amju
{
struct Grade;

// * Feedback Balloon *
// Show feedback on how good a note attempt was.

// Display feedback in the given gui tree.
void SetUpFeedbackBalloon(const Grade& g, PGuiElement gui);

// Just set visibility on balloon: TODO Should be internal, so we can
//  have circular buffer of these.
void SetFeedbackBalloonVisible(bool showNotHide, PGuiElement gui);

}

