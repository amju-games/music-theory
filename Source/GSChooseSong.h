#pragma once

#include "GSShowGui.h"

namespace Amju
{
// ** Choose song **
// We are building a GUI in code, and to look at extents, we
//  inherit from GSShowGui.
class GSChooseSong : public GSShowGui
{
public:
  GSChooseSong();

  void OnActive() override;
  void Draw2d() override;

  // Callback for when we stop on a song in the scrolling list.
  void OnTabStop(int tabStop);

protected:
  void InitGui();
  void InitQuitButton();
  void InitScrollingGui();

protected:
  int m_lastTabStop = 0; // track most recent tab stop in scrolling list.
};

// Use this to move title up if on two lines; poor substitute for 
//  decent vertical justification
void MoveUpMultiLineTitle(GuiTextBase* t);

using TheGSChooseSong = Singleton<GSChooseSong>;
}

