// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#pragma once

#include <Singleton.h>
#include <GameState.h>

namespace Amju 
{
// Copy config file(s) from the glue file to the Save Dir.

class GSCopyAssets : public GameState
{
  GSCopyAssets();
  friend class Singleton<GSCopyAssets>;

public:
  virtual void Update();
  virtual void Draw();
  virtual void Draw2d();
  virtual void OnActive();

};
typedef Singleton<GSCopyAssets> TheGSCopyAssets;
} // namespace

