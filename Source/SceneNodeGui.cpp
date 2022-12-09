#include <AmjuGL.h>
#include "SceneNodeGui.h"
#include "UseVertexColourShader.h"

namespace Amju
{
const char* SceneNodeGui::NAME = "gui";

void SceneNodeGui::Update()
{
  m_gui->Update();
}

void SceneNodeGui::Draw() 
{
  AmjuGL::PushAttrib(AmjuGL::AMJU_LIGHTING | AmjuGL::AMJU_DEPTH_READ | AmjuGL::AMJU_BLEND);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  AmjuGL::Disable(AmjuGL::AMJU_LIGHTING);
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::MultMatrix(m_local);

#if defined(WIN32) || defined(MACOSX)
  UseVertexColourShader(); // Bah, for desktop OpenGL
#endif

  m_gui->Draw();

#if defined(WIN32) || defined(MACOSX)
  AmjuGL::UseShader(nullptr); // ?
#endif

  AmjuGL::PopMatrix();
  AmjuGL::PopAttrib();
}

bool SceneNodeGui::Load(File* f) 
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected scene node name");
    return false;
  }

  std::string guiFilename;
  if (!f->GetDataLine(&guiFilename))
  {
    f->ReportError("Expected GUI filename");
    return false;
  }

  m_gui = LoadGui(guiFilename, false);
  if (!m_gui)
  {
    f->ReportError("Failed to load GUI " + guiFilename);
    return false;
  }

  if (!SceneNode::LoadMatrix(f))
  {
    return false;
  }

  return true;
}
}

