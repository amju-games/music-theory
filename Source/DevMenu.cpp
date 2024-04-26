#include <EventPoller.h>
#include <GuiMenu.h>
#include "DevMenu.h"
#include "GSGuiEdit.h"
#include "GSShowGui.h"
#include "GSTitle.h"

namespace Amju
{
template<class STATE>
static void GoToState(GuiElement*)
{
  GSBase* currentState = dynamic_cast<GSBase*>(TheGame::Instance()->GetState());
  if (currentState)
  {
    STATE* a = Singleton<STATE>::Instance();
    if (a->GetPrevState() != currentState)
    {
      a->SetPrevState(currentState);
    }
//    a->SetEditGuiFilename(currentState->GetGuiFilename());
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
//    gotoSubmenu->AddChild(new GuiTextMenuItem("Gui Edit", GoToState<GSGuiEdit>));
    gotoSubmenu->AddChild(new GuiTextMenuItem("Show Gui", GoToState<GSShowGui>));
    gotoSubmenu->AddChild(new GuiTextMenuItem("Title", GoToState<GSTitle>));

    gm->AddChild(new GuiNestMenuItem("Go to    ", gotoSubmenu));
  }
  return gm;
}
}