// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <ParticleEffect2d.h>

namespace Amju
{
// * ParticleFx *
// General-purpose particle effect for this game.
// TODO Come up with a better way of specialising/authoring 
//  particle effects
class ParticleFx : public ParticleEffect2d
{
public:
  static void AddToFactory();

  Vec3f NewVel() const override;
  Vec3f NewAcc() const override;
  float NewRotVel() const override;
};
}

