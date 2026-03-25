#pragma once

#include <GuiSprite.h>

namespace Amju
{
// * GuiPatch *
// 9-patch: splits a sprite cell into 4 and stretches the middle
//  in x and y.
class GuiPatch : public GuiSprite
{
public:
  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  void Draw() override;
  bool Load(File*) override;

private:
  void AddToTrilist(AmjuGL::Tris& tris) override;

private:
  // This is the size of each corner square; a quadrant from
  //  the given cell is mapped to each corner. 
  // (The gaps are filled by stretching the quadrant edges to fit.)
  Vec2f m_cornerSize;
};
}

