// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuGL.h>
#include <CursorManager.h>
#include <GuiButton.h>
#include <GuiComposite.h>
#include <GuiDecAnimation.h>
#include <GuiPoly.h> // to set global texture on poly outlines
#include <GuiText.h> // set version string
#include <Timer.h>
#include "GSBase.h"
#include "AutoRepeatFilter.h"
#include "AutoTest.h"
#include "BassMidiInput.h"
#include "GetVersion.h"
#include "KeyInputHandler.h"
#include "MyROConfig.h"
#include "PlayMidi.h"
#include "PlayWav.h"
#include "PrintGui.h"
#include "ShareManager.h"
#include "UseVertexColourShader.h"

// By default, frame stats are off for release builds.
#ifdef _DEBUG
#define YES_FRAME_STATS
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
 
  // Extra GUI, to display frame stats, etc
  // MIDI connect/disconnect too - so we want to add it
  //  to every state's gui, whether or not we display frame stats.
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
    Assert(false);
  }

  // Show MIDI connection status.
  // We don't want to show the MIDI gui every state change, so
  //  we pause the anim if no change.
  TriggerMidiConnectGui();

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
  
  added = kih.AddHandler(MakeKeyEvent('i'),
    [this](const KeyEvent&)->bool
    {
      static int connections = 0;
      TestMidiConnectGui((connections++) % 3);
      return true;
    },
    "Trigger MIDI connect GUI");
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

void GSBase::TestMidiConnectGui(int numConnections)
{
  MidiConnectGuiImpl(numConnections);
}

void GSBase::TriggerMidiConnectGui()
{
  auto midiInput = GetMidiInput();
  Assert(midiInput);
  MidiConnectGuiImpl(midiInput->GetNumConnections());
}

void GSBase::MidiConnectGuiImpl(int numConnections)
{
  // This is called when one of two things change:
  // 1. The GameState has changed and we have loaded a new GUI
  // 2. The MIDI connection has changed.
  // We only want to do an animation for case 2.
  // But if we just want to show the status without an anim, we
  //  want to set the correct gui to show.
  
  auto root = dynamic_cast<GuiDecAnimation*>(GetElementByName(m_gui, "midi-anim-root"));
  Assert(root);

  static int prevNumConnections = -1;
  if (prevNumConnections == numConnections)
  {
    // No change - don't do an anim, but do set a static status gui.
    // TODO
    root->SetIsPaused(true);
    return;
  }
  // Store new value
  prevNumConnections = numConnections;

  Assert(m_gui);
  auto chooser = dynamic_cast<GuiDecAnimation*>(m_gui->GetElementByName("midi-choose-anim"));
  Assert(chooser);

  // Set const anim value depending on connected status.
  bool isConnected = numConnections > 0;
  chooser->SetValue(isConnected ? 1.f : 0.f);

  // Play connected or disconnected wav
  // TODO maybe piano notes
  PlayWav(isConnected ? "blip-up" : "blip-down");

  // Reset animation and unpause
  root->ResetAnimation();
  root->SetIsPaused(false);
}

void GSBase::OnDeviceChangeEvent(const DeviceChangeEvent&)
{
  auto m = GetMidiInput();
  if (m)
  {
    m->OnDeviceChange();
    // TODO trigger animation for connected or disconnected.
    TriggerMidiConnectGui();
  }
}
}

