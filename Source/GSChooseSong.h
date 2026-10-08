// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#pragma once

#include "GSShowGui.h"

namespace Amju
{
class GuiTextBase;

// ** Choose song **
// We are building a GUI in code, and to look at extents, we
//  inherit from GSShowGui.
class GSChooseSong : public GSShowGui
{
public:
  GSChooseSong();

  void OnActive() override;
  void Draw2d() override;

  void AutoTestSetup() override;

  // Callback for when we stop on a song in the scrolling list.
  void OnTabStop(int tabStop);

protected:
  void InitGui();
  void InitLRButtons();
  void InitQuitButton();
  void InitScrollingGui();

protected:
  // Track most recent tab stop in scrolling list.
  // Start with invalid value, set on first activation.
  int m_lastTabStop = 999;

  // This is the last position, (the final song displayed!)
  int m_finalTabStop = 0;
};

// Use this to move title up if on two lines; poor substitute for 
//  decent vertical justification
void MoveUpMultiLineTitle(GuiTextBase* t);

using TheGSChooseSong = Singleton<GSChooseSong>;
}

