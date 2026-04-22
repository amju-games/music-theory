#include <GuiText.h>
#include <Md2Model.h>
#include <SceneGraph.h>
#include <SoundManager.h>
#include <Timer.h>
#include "Consts.h"
#include "GSChooseSong.h"
#include "GSHero.h"
#include "GS3dTitle.h"
#include "Md2SceneNode.h"
#include "PlayWav.h"
#ifdef AMJU_IOS
#include "Version.h"
#endif

namespace Amju
{
static void OnStart(GuiElement* elem)
{
  TheSoundManager::Instance()->StopSong(); // TODO play next song

  PlayWav(WAV_START_BUTTON); // This wav should blend nicely with the title music
  TheGSChooseSong::Instance()->SetPrevState(TheGS3dTitle::Instance());
  GoTo<TheGSChooseSong>();
}

GS3dTitle::GS3dTitle()
{
 m_sceneFilename = "Scene/3d-title-scene.txt";
 m_guiFilename = "Gui/gs_3dtitle.txt";
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

#ifdef AMJU_IOS
  // Set version text (TODO move to a better place)
  auto versionText = dynamic_cast<GuiTextBase*>(GetElementByName(m_gui, "version-text"));
  Assert(versionText);
  versionText->SetText(VERSION_STRING);
#endif

  auto pianoNode = GetSceneGraph()->GetRootNode(SceneGraph::AMJU_OPAQUE)->
    GetNodeByName("piano");
  auto pianoMd2 = dynamic_cast<Md2SceneNode*>(pianoNode);
  Assert(pianoMd2);
  m_piano.SetMd2Node(pianoMd2);
}

void GS3dTitle::Update()
{
  GSBase3d::Update();

  // Get the camera node, track backwards (through the town)
  // Not animated in scene node data because there aren't animation
  //  nodes, etc. :(

  // The scale of the town is: 1 unit high, 4 units wide.

  const float dt = TheTimer::Instance()->GetDt();
  const float VEL = -10.f; // Units/sec. 

  // Get the camera node
  SceneGraph* sg = GetSceneGraph();
  auto camera = dynamic_cast<SceneNodeCamera*>(sg->GetRootNode(SceneGraph::AMJU_OPAQUE)->GetNodeByName("camera")); //GetCamera(); // ?
  auto pos = camera->GetEyePos();
  pos.z -= (dt * VEL);
  camera->SetEyePos(pos);

  m_piano.Update();
}

void GS3dTitle::Piano::SetMd2Node(Md2SceneNode* md2)
{
  m_sceneNode = md2;

  // The model just has static/const data; no current anim frame etc.
  auto model = md2->GetMd2();

  m_stand = model->GetAnimationFromName("stand");
  m_open = model->GetAnimationFromName("open");
  m_close = model->GetAnimationFromName("close");

  model->SetDoesFreeze(m_stand, true); // these anims don't loop!
  model->SetDoesFreeze(m_open, true);
  model->SetDoesFreeze(m_close, true);

  md2->SetAnim(m_stand);

  Reset();
}

void GS3dTitle::Piano::Update()
{
  m_elapsedTime += TheTimer::Instance()->GetDt();
  
}

void GS3dTitle::Piano::Reset()
{
  m_elapsedTime = 0;
}
}

