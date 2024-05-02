#pragma once

#include "GSPages.h"

namespace Amju
{
// * GSPagesEdit *
// Display pages, with info, allows me to edit Qs etc.
class GSPagesEdit : public GSPages
{
public:
  //
  void OnActive() override;
  bool OnKeyEvent(const KeyEvent&) override;

protected:
  void NextPage() override;
};

using TheGSPagesEdit = Singleton<GSPagesEdit>;
}
