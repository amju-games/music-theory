// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include "LurkMsg.h"

namespace Amju
{
class Notebook : public LurkMsg
{
public:
  Notebook();

  void Draw() override;
  void Update() override;

  void DoOk() override; // Lurker calls this when we close Notebook

private:
  int m_pageNum = 0; // 0 is cover, 1 is page 1, etc.??? OR 0 is TOC???
};
}
