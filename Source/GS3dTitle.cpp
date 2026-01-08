#include "GS3dTitle.h"

namespace Amju
{
GS3dTitle::GS3dTitle()
{
 m_sceneFilename = "Scene/3d-title.txt";
 m_guiFilename = "Gui/3d-title.txt";
}

void GS3dTitle::Update()
{
  GSBase3d::Update();
}
}

