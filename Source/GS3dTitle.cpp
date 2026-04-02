#include <SceneGraph.h>
#include <SoundManager.h>
#include <Timer.h>
#include "Consts.h"
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GS3dTitle.h"
#include "PlayWav.h"

namespace Amju
{
static void OnStart(GuiElement* elem)
{
  TheSoundManager::Instance()->StopSong(); // TODO play next song

  PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music
  TheGSHero::Instance()->SetPrevState(TheGS3dTitle::Instance());
  GoTo<TheGSChooseSong>();
}

GS3dTitle::GS3dTitle()
{
 m_sceneFilename = "Scene/title-scene.txt";
 m_guiFilename = "Gui/3d-title.txt";
}

void GS3dTitle::OnActive()
{
  GSBase3d::OnActive();

  // Start playing title music
  auto sm = TheSoundManager::Instance();
  sm->SetSongMaxVolume(0.1f); // for some reason it's deafening on Mac
  sm->PlaySong("Music/amt-title.it");

  // Set up start button
  GuiElement* startButton = GetElementByName(m_gui, "start-button");
  startButton->SetCommand(OnStart);
  startButton->SetHasFocus(true);
}

void GS3dTitle::Update()
{
  GSBase3d::Update();

  // Get the camera node, track backwards (through the town)
  // Not animated in scene node data because there aren't animation
  //  nodes, etc. :(

  // The scale of the town is: 1 unit high, 4 units wide.

  const float dt = TheTimer::Instance()->GetDt();
  const float VEL = 1.f; // Units/sec. 

  // Get the camera node
  SceneGraph* sg = GetSceneGraph();
  auto camera = dynamic_cast<SceneNodeCamera*>(sg->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("camera")); //GetCamera(); // ?
  auto pos = camera->GetEyePos();
  pos.z -= (dt * VEL);
  camera->SetEyePos(pos);
}
}

