// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <string>
#include <GuiElement.h>

namespace Amju
{
// Displays an integer value in the HUD.
// Increments the currently displayed value until it reaches the desired
//  (internal) number. As we update the text node we reset its animation
//  node (if there is one) so the text animates when incremented.
struct HudNumber
{
  int m_displayedNumber = 0;
  int m_internalNumber = 0;
  int m_increment = 0; // add this each time increment 
  PGuiElement m_guiTextElement; // GuiText node displaying the value
  PGuiElement m_guiAnimElement; // animation node which animates the text

  // Prefix and suffix strings to add to the integer value
  std::string m_prefix;
  std::string m_suffix;

  // Call once after initialising GUI
  void SetGuiElement(
    PGuiElement guiRoot, 
    const std::string& guiTextElementName, 
    const std::string& guiAnimElementName = ""); // name of animation node, if there is one

  // Call to immediately reset value, e.g. just after calling the above, 
  //  and any other time the value should be reset.
  void Reset(int initialValue = 0); 

  // If there is one, trigger the animation controller for this HUD value --
  //  this is called in Add.
  void ResetAnimation();

  // Call every frame
  void Update();

  // Call to add to the internal value (amount to add can be negative).
  // Specify how many frames to take to update.
  void Add(int amountToAdd, int numFrames);

  // After a pause, we will have new GUI elements so update text value
  //  to internal value.
  void ResumeAfterPause();
};

/*
// Update a number displayed in the HUD.
void NumUpdate(PGuiElement gui, const std::string& elemName, int value);

// Update a string displayed in the HUD.
void NumUpdate(PGuiElement gui, const std::string& elemName, const std::string& value);
*/
}

