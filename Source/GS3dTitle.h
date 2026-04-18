#pragma once

#include <Singleton.h>
#include "GSBase3d.h"

namespace Amju
{
class Md2SceneNode;

class GS3dTitle : public GSBase3d
{
public:
  GS3dTitle();

  void Update() override;
  void OnActive() override;

protected:
  struct Piano
  {
    void SetMd2Node(Md2SceneNode* md2);
    void Update();
    void Reset();

    float m_elapsedTime = 0;
    int m_stand = 0;
    int m_open = 0;
    int m_close = 0;
    Md2SceneNode* m_sceneNode = nullptr;
  };

  Piano m_piano;
};

using TheGS3dTitle = Singleton<GS3dTitle>;
}
