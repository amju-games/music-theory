// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include <GuiMenu.h>
#include <GuiPoly.h> // to set global texture on poly outlines
#include "GSBase.h"
#include "KeyInputHandler.h"
#include "MyROConfig.h"
#include "PrintGui.h"
#include "ShareManager.h"
#include "UseVertexColourShader.h"

#define YES_FRAME_STATS
// Not on device/release tho, right?
#if defined(AMJU_IOS) && !defined(_DEBUG)
#undef YES_FRAME_STATS
#endif 

namespace Amju
{
static bool reload = false;

void OnShare(GuiElement*)
{
  TheShareManager::Instance()->ShareTextAndScreenshot();
}

void GSBase::HideButtons(GuiElement* elem)
{
  if (dynamic_cast<GuiButton*>(elem))
  {
    elem->SetVisible(false);
  }
  else if (GuiComposite* comp = dynamic_cast<GuiComposite*>(elem))
  {
    int n = comp->GetNumChildren();
    for (int i = 0; i < n; i++)
    {
      HideButtons(comp->GetChild(i));
    }
  }
}

GSBase* GSBase::HideButtons()
{
  HideButtons(m_gui);
  return this;
}

void GSBase::Update()
{
#ifdef _DEBUG
  if (reload)
  {
    reload = false;
    TheMessageQueue::Instance()->Clear();
    ReloadMyROConfig();
    ReloadGui();
  }
#endif

  if (m_gui)
  {
    m_gui->Update();
  }

// Not on device?
#ifdef YES_FRAME_STATS
  auto frameStatsText = 
    dynamic_cast<IGuiText*>(GetElementByName(m_gui, "frame-stats"));
  if (frameStatsText)
  {
    frameStatsText->SetText(TheGame::Instance()->GetFrameStats());
  }
#endif
}

void GSBase::Draw2d() 
{
  AmjuGL::SetClearColour(Colour(.95f, .95f, .95f, 1.f));

  if (m_gui)
  { 
    UseVertexColourShader();
    m_gui->Draw();
  }

#ifdef GEKKO
  TheCursorManager::Instance()->Draw();
#endif
}

void GSBase::OnActive() 
{
  GameState::OnActive();

  // Add qwerty-keyboard key bindings we want for this state.
  AddKeyInputHandlers();

  // Report on keys currently mapped
std::cout << "** KEY BINDINGS:\n" 
  << GetKeyInputHandler().ListHandlers() << "\n";

  IGuiPoly::SetPolyOutlineTextureName("Image/white.png");
 
  Assert(!m_guiFilename.empty()); // set gui filename in ctor pls!
  m_gui = LoadGui(m_guiFilename);
  if (!m_gui)
  {
    std::cout << "Failed to load: " << m_guiFilename << "\n";
    Assert(false);
  }
 
// Not on device?
#ifdef YES_FRAME_STATS
  // Extra GUI, to display frame stats, etc
  auto extraGui = LoadGui("Gui/extra-gui.txt", false);
  if (extraGui)
  {
    auto newRoot = new GuiComposite;
    newRoot->AddChild(m_gui);
    newRoot->AddChild(extraGui);
    newRoot->SetName("GUI root node, created in GSBase.");
    m_gui = newRoot;
  }
  else
  {
    std::cout << "Failed to load extra GUI.\n";
  }
#endif
}

GuiElement* GSBase::GetGui()
{
  return m_gui;
}

void GSBase::OnDeactive()
{
  RemoveKeyInputHandlers();

  // Anim messages in the queue need to be cleared!
  TheMessageQueue::Instance()->Clear();

  GameState::OnDeactive();
  m_gui.Reset(); // Reset any weak ptrs to bits of the gui first!
}

void GSBase::ReloadGui()
{
  // Deactivate and reactivate the current state, causing a reload.
  OnDeactive();
  OnActive();
}

KeyInputHandler& GSBase::AddKeyInputHandlers()
{
  auto& kih = GetKeyInputHandler();

  bool added = true;

#ifdef _DEBUG
  added = kih.AddHandler(MakeKeyEvent('B'), 
    [](const KeyEvent&)->bool 
    {
      auto* state = TheGame::Instance()->GetState();
      if (state->GetPrevState())
      {
        state->GoBack();
        return true;
      }
      return false;
    },
    "Go back to previous state");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('P'), 
    [](const KeyEvent&)->bool 
    {
      TheGame::Instance()->PauseGame();
      return true;
    },
    "Pause game");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('T'), 
    [](const KeyEvent&)->bool 
    {
      TheResourceManager::Instance()->Reload();
      return true;
    },
    "Reload all resources");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('Y'), 
    [](const KeyEvent&)->bool 
    {
      TheResourceManager::Instance()->DebugPrint();
      AmjuGL::ReportState(std::cout);
      return true;
    },
    "Print state of resources and AmjuGL");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('R'), 
    [&](const KeyEvent&)->bool 
    {
      reload = true;
      return true;
    },
    "Reload GUI");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('G'), 
    [this](const KeyEvent&)->bool 
    {
      if (m_gui)
        PrintGui(m_gui);
      else
        std::cout << "Null GUI!\n";
      return true;
    },
    "Print GUI tree");
  Assert(added);

#endif // _DEBUG

  return kih;
}

void GSBase::RemoveKeyInputHandlers()
{
  GetKeyInputHandler().Clear();
}

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
  return GetKeyInputHandler().OnKeyEvent(ke);
}

const std::string& GSBase::GetGuiFilename()
{
  return m_guiFilename;
}
}

