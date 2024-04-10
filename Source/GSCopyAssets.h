// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

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

// Copy asset files from Glue file into writable "Save dir"
bool CopyFromGlueFile(const std::string& destDir);
} // namespace

