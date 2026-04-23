// * PIANO FEST *
// (c) Copyright 2026 Juliet Colman

#pragma once

#include <Singleton.h>
#include "GSBase.h"

namespace Amju
{
// * GSConfirmSong *
// Show song selected in main menu.
// Options to go back to main menu, etc.
class GSConfirmSong : public GSBase
{
public:
  GSConfirmSong();

  void OnActive() override;

private:
  void InitButtons();
  void InitText();
  void InitScore();
  void StartSong();
};

typedef Singleton<GSConfirmSong> TheGSConfirmSong;
}

