// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "GSBase.h"
#include <Singleton.h>

namespace Amju
{
class GuiMusicScore;

// * GSTestShowScore *
// Test: show static display of music score, loaded from file.
// This is used to show the results from MakeScore: see 
//  MakeScore/Examples. We use an 'empty score' file, concat the
//  makescore output, then run this state with the concatenated
//  gui file as the 3rd command-line param.
class GSTestShowScore : public GSBase
{
public:
  GSTestShowScore();

  void OnActive() override;
};

typedef Singleton<GSTestShowScore> TheGSTestShowScore;
}
