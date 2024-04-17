#include <EventPoller.h>
#include <GuiMenu.h>
#include "DevMenu.h"
#include "GSGuiEdit.h"

namespace Amju
{
static void GoToEditState(GuiElement*)
{
  GSBase* currentState = dynamic_cast<GSBase*>(TheGame::Instance()->GetState());
  if (currentState)
  {
    GSGuiEdit* a = TheGSGuiEdit::Instance();
    if (a->GetPrevState() != currentState)
    {
      a->SetPrevState(currentState);
    }
    a->SetEditGuiFilename(currentState->GetGuiFilename());
    TheGame::Instance()->SetCurrentState(a);
  }
}

GuiMenu* GetDevMenu()
{
  static GuiMenu* gm = nullptr;
  if (!gm)
  {
    gm = new GuiMenu();
    TheEventPoller::Instance()->AddListener(gm);

    gm->SetIsVertical(false);
    gm->SetLocalPos(Vec2f(-1.0f, 1.0f));

    GuiMenu* gotoSubmenu = new GuiMenu;
    gotoSubmenu->AddChild(new GuiMenuItem("Gui Edit", GoToEditState));

    gm->AddChild(new GuiNestMenuItem("Go to    ", gotoSubmenu));
  }
  return gm;
}
}