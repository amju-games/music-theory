#pragma once

namespace Amju
{
// * AnimListener *
// MIXIN class
// Recieves notifications from e.e. Md2SceneNode when an anim ends.
class AnimListener
{
public:
  virtual ~AnimListener() = default;
  virtual void OnAnimFinished() {}
  virtual void OnAnimFreeze() {}
  virtual void OnAnimRepeat() {}
};
}

