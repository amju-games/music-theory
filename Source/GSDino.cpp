#include "GSDino.h"
#include "SceneTimeline.h"

namespace Amju
{
void GoViaDino(GameState* gs, const std::string& dinoScript)
{
  auto dino = TheGSDino::Instance();
  dino->SetNextState(gs);
  dino->LoadScript(dinoScript);
  TheGame::Instance()->SetCurrentState(dino);
}

GSDino::GSDino()
{
  m_sceneFilename = "Scene/dino-chat-scene.txt";
  m_guiFilename = "Gui/gs_dino.txt";
  m_pendingScriptFile = "dino-test.txt";
}

void GSDino::SetNextState(GameState* gs)
{
  m_nextState = gs;
}

void GSDino::SetPendingScriptFile(const std::string& scriptFile)
{
  m_pendingScriptFile = scriptFile;
}

void GSDino::Update()
{
  // TODO Hard code fitting the dino's shadow to the dino's pos...
  //  OR should we create Game Objects to do it properly??

  GSBase3d::Update();
}

void GSDino::OnActive()
{
  GSBase3d::OnActive(); // Does not automatically load 3d scene. WHY!?
  Reload3d();

  if (!m_pendingScriptFile.empty())
  {
    LoadScript(m_pendingScriptFile);
    m_pendingScriptFile.clear();
  }
}

bool GSDino::LoadScript(const std::string& scriptFile)
{
  // A script is a scene timeline, decoupled from the rest of the scene.
  auto fullFilename = "timelines/" + scriptFile;

  // We want some special timeline event types, like dino-text or sm?
  // All the text will end up in the string table, of course. We could
  //  have multiple alternatives which we cycle through or pick randomly. 

  // We don't need a finish timeline event. When the timeline is done,
  //  we can get a callback, right? Although, the messages are just in
  //  the queue.. 

  //auto sg = GetSceneGraph(); // ?
  // TODO Pass in sg to make it game-agnostic
  auto node = new SceneTimelineAppend;
  File f;
  if (!f.OpenRead(fullFilename)) return false;
  if (!node->Load(&f))
  {
    return false;
  }

  return true;
}
}

