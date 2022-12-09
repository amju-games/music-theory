// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <SceneNodeFactory.h>
#include "ParticleFx.h"

namespace Amju
{
void ParticleFx::AddToFactory()
{
  TheSceneNodeFactory::Instance()->Add(NAME, 
    []() -> SceneNode*
    {
      return new ParticleFx;
    }
  );
}

Vec3f ParticleFx::NewVel() const 
{
  return Vec3f(1.0f, 1.0f, 0);
}

Vec3f ParticleFx::NewAcc() const 
{
  return Vec3f(0, -1.0f, 0);
}
}
