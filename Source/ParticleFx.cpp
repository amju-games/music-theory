// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <AmjuRand.h>
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
  return Vec3f(Rnd(-1.f, 1.f), Rnd(-0.5f, 1.f), Rnd(0, 1.f));
}

Vec3f ParticleFx::NewAcc() const 
{
  return Vec3f(0, -1.0f, 0);
}

float ParticleFx::NewRotVel() const
{
  return Rnd(-10.f, 10.f);
}
}
