// * Amju PIANO FEST *
// (c) Copyright Juliet Colman 2000-2026

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiPoly.h> // to set global texture on poly outlines
#include <GuiText.h> // set version string
#include <Timer.h>
#include "GSBase.h"
#include "AutoRepeatFilter.h"
#include "AutoTest.h"
#include "GetVersion.h"
#include "KeyInputHandler.h"
#include "MyROConfig.h"
#include "PFScreenshot.h"
#include "PlayMidi.h"
#include "PrintGui.h"
#include "ShareManager.h"
#include "UseVertexColourShader.h"

// By default, frame stats are off for release builds.
#ifdef _DEBUG
#define YES_FRAME_STATS
#endif

namespace Amju
{
static bool s_reload = false;
static bool s_showFrameStats = true;

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

GuiButton* GSBase::FindFocusButton(GuiElement* elem)
{
  if (auto button = dynamic_cast<GuiButton*>(elem)) 
  {
    if (button->IsFocusButton())
    {
      return button;
    }
  }
  else if (GuiComposite* comp = dynamic_cast<GuiComposite*>(elem))
  {
    int n = comp->GetNumChildren();
    for (int i = 0; i < n; i++)
    {
      if (auto button = FindFocusButton(comp->GetChild(i)))
      {
        return button;
      }
    }
  }
  return nullptr;
}

void GSBase::AutoTestSetup()
{
  const float DELAY = 0.3f;
  AutoMsg([this]()
  { 
    // Try to find a button with Focus. If we find one, click it.
    if (auto button = FindFocusButton(m_gui))
    {
      std::cout << "*** AUTO TEST *** Found Focus button \""
        << button->GetName()
        << "\", pressing it...\n";
      // Simulate button press
      button->ExecuteCommand(); 
    }
  }, DELAY);
}

void GSBase::SetVersionText()
{
  Assert(m_gui);
  auto versionText = dynamic_cast<GuiTextBase*>(
    m_gui->GetElementByName("version-text"));
  if (versionText)
  {
    versionText->SetText("v. " + GetVersionString3());
  }
}

void GSBase::Update()
{
#ifdef _DEBUG
  if (s_reload)
  {
    s_reload = false;
    TheMessageQueue::Instance()->Clear();
    ReloadMyROConfig();
    ReloadGui();
  }
#endif

  if (m_gui)
  {
    m_gui->Update();
  }

#ifdef YES_FRAME_STATS
  auto frameStatsText = 
    dynamic_cast<IGuiText*>(GetElementByName(m_gui, "frame-stats"));
  if (frameStatsText)
  {
    frameStatsText->SetText(
      s_showFrameStats ? TheGame::Instance()->GetFrameStats() : "");
  }
#endif

  m_timeInThisState += TheTimer::Instance()->GetDt();
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

  m_timeInThisState = 0;

  // Add qwerty-keyboard key bindings we want for this state.
  ClearAutoRepeatFlags();
  AddKeyInputHandlers();

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

  // If autotest is turned on, set up testing for this state.
  // We check here if tests are disabled, so in subclasses, we
  //  know tests are enabled if AutoTestSetup() is called.
  if (GetAutoTestLevel() != AutoTestLevel::AMJU_NO_TEST)
  {
    // The idea here is that every state knows how to test itself;
    //  so it shouldn't matter what order states get activated.
    //  We'll see if that theory pans out.
    AutoTestSetup();
  }
}

GuiElement* GSBase::GetGui()
{
  return m_gui;
}

void GSBase::OnDeactive()
{
  RemoveKeyInputHandlers();
  ClearAutoRepeatFlags();

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
  added = kih.AddHandler(MakeKeyEvent('/'), 
    [&](const KeyEvent&)->bool 
    {
      std::cout << "** Key mappings:\n" << kih.ListHandlers() << "\n";
      return true;
    },
    "Print key mappings");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('0'),
    [](const KeyEvent&)
    {
      auto path = SavePFScreenshot();
      std::cout << "Saved screenshot to: " << path << "\n";
      return true;
    },
    "Save screenshot");
  Assert(added);

  added = kih.AddHandler(MakeKeyEvent('8'),
    [](const KeyEvent&)->bool
    {
      s_showFrameStats = !s_showFrameStats;
      return true;
    },
    "Toggle frame stats display");
  Assert(added);

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
      s_reload = true;
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

  added = kih.AddHandler(MakeKeyEvent('H'),
    [&](const KeyEvent&)->bool
    {
      HideButtons();
      return true;
    },
    "Hide buttons");
  Assert(added);

#endif // _DEBUG

#ifdef CRASH_TEST
  // This should be in a test release build, but not actually shipped, ha ha
  bool crasher = kih.AddHandler(MakeKeyEvent('9'),
    [](const KeyEvent&)->bool
    {
      volatile int* ptr = nullptr;
      *ptr = 42;
      return true;
    },
    "Force a crash, for BugSplat testing");
#endif // CRASH_TEST

  return kih;
}

void GSBase::RemoveKeyInputHandlers()
{
  GetKeyInputHandler().Clear();
}

bool GSBase::OnKeyEvent(const KeyEvent& ke)
{
  auto res = GetKeyInputHandler().OnKeyEvent(ke);
  return res == KeyInputHandler::Result::AMJU_KEY_EVENT_CONSUMED;
}

const std::string& GSBase::GetGuiFilename()
{
  return m_guiFilename;
}

void GSBase::OnMusicKbEvent(const MusicKbEvent& musicEvent)
{
  // We have recvd a music event from virtual piano, MIDI input
  //  or qwerty keys.

  PlayMidi(musicEvent.m_note, musicEvent.m_velocity);
}
}

