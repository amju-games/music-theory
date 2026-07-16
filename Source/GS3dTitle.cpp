#include <GuiText.h>
#include <Md2Model.h>
#include <SceneGraph.h>
#include <SoundManager.h>
#include <Timer.h>
#include "BassPlayMidi.h"
#include "Consts.h"
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GS3dTitle.h"
#include "Md2SceneNode.h"
#include "MySceneGraph.h"
#include "PlayWav.h"
#include "Version.h"

namespace Amju
{
static void OnStart(GuiElement* elem)
{
  TheSoundManager::Instance()->StopSong(); // TODO play next song

  PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music
  TheGSChooseSong::Instance()->SetPrevState(TheGS3dTitle::Instance());
  TheMessageQueue::Instance()->Clear(); // kill timeline events
  GoTo<TheGSChooseSong>();
}

GS3dTitle::GS3dTitle()
{
  m_sceneFilename = "Scene/3d-title-scene.txt";
  m_guiFilename = "Gui/gs_3dtitle.txt";
}

void GS3dTitle::OnDeactive()
{
  // This title state is quite resource heavy, what with the MD2s etc.
  // So try dumping the current resources. This is a two-step process:
  // 1. Clear the res mananger
  // 2. Zero the shared ptrs to the resources.

  TheResourceManager::Instance()->Clear();
  GetSceneGraph()->Clear(); // NOT done in GSBase3d::OnDeactive
  StopMidiSong(); // TODO Fade?!

  GSBase3d::OnDeactive();
}

void GS3dTitle::OnActive()
{
  GSBase3d::OnActive(); // Does not automatically load 3d scene.
  Reload3d();

  // Start playing title music is performed by a timeline event

  // Set up start button
  GuiElement* startButton = GetElementByName(m_gui, "start-button");
  startButton->SetCommand(OnStart);
  startButton->SetHasFocus(true);

  // Set version text (TODO move to a better place)
  auto versionText = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "version-text"));
  Assert(versionText);
  versionText->SetText(VERSION_STRING);
}

void GS3dTitle::Update()
{
  GSBase3d::Update();

  // Get the camera node, track backwards 
  // Not animated in scene node data because there aren't animation
  //  nodes, etc. :(
  // TODO Add animation node(s) and timeline events to control it

  const float dt = TheTimer::Instance()->GetDt();
  const float VEL = -10.f; // Units/sec. 

  // Get the camera node
  SceneGraph* sg = GetSceneGraph();
  auto camera = dynamic_cast<SceneNodeCamera*>(
    sg->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("camera")); 

  // Move eye pos - TODO animations should be applicable to scene nodes,
  //  not just Gui nodes.
  auto pos = camera->GetEyePos();
  pos.z -= (dt * VEL);
  camera->SetEyePos(pos);
}
}

