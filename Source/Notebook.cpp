// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#include <Batched.h>
#include <GuiComposite.h>
#include "Notebook.h"

namespace Amju
{
Notebook::Notebook()
{
  m_isModal = true;
  m_lurkPos = AMJU_CENTRE;
  m_gui = ::Amju::LoadGui("Gui/notebook.txt");
  Assert(m_gui);

  // Set as modal listener
  SetAsListener(m_gui);

  GuiButton* quitButton = dynamic_cast<GuiButton*>(GetElementByName(m_gui, "quit-button"));
  Assert(quitButton);
  quitButton->SetCommand([](GuiElement*) { TheLurker::Instance()->OnLurkOk(); });

  // TODO TEMP TEST
  // Load a notebook page
  PGuiElement page = ::Amju::LoadGui("Gui/tap-picture-treb-clef.txt");
  GuiComposite* comp = dynamic_cast<GuiComposite*>(GetElementByName(m_gui, "page-anchor"));
  Assert(comp);
  comp->AddChild(page);
}

void Notebook::Draw()
{
  DrawModalBg();

  // Flush, to make sure that notebook will appear over the top of 
  //  everything else.
  Batched::DrawAll();

  m_gui->Draw();
}

void Notebook::Update()
{
  m_gui->Update();

  // LURK_HIDING state set when we execute LurkOk command
  if (m_state == LurkMsg::LURK_HIDING)
  {
    // TODO Scroll off over time

    SetAsListener(nullptr); // stop being the modal listener
    m_state = LurkMsg::LURK_FINISHED; // now this msg will get popped off
  }
}
}
