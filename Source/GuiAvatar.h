// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#pragma once

#include <GuiComposite.h>

namespace Amju
{
class GuiAvatar : public GuiComposite
{
public:
  static void AddToFactory();

  bool Load(File*) override;

  // Set Avatar from string.
  // We set descendant nodes from a string like this:
  // "colour; iris - colour; 005678 | scale; head - scale; 1.0, 1.2 | ... "
  //  1. Split using |
  //  2. This gives 3 strings separated with ;
  //  3. First is type, so we know what to dynamic_cast to (or could use a factory ?)
  //  4. Second is name of GuiElement we will set attribs on
  //  5. Third is value of attrib - how we set it depends on the type (string 1), 
  //     so again a factory could be the best way.
  bool SetFromString(const std::string&);

  void Update() override;

  // Call this to set a value on one descendant node. This is used when we set
  //  the avatar from string, and can be used to animate or alter the avatar later.
  // Format of string is "type;name;value", e.g. "colour; iris-colour; 005678"
  // The type should use names used by GuiFactory where possible, for 
  //  consistency - no point having a second set of different names for things.
  // Name is the name of the GuiElement. 
  // Value depends on the type, e.g. for a colour, it's a hex rgb[a] value.
  bool SetOneDescendant(const std::string&);

  void SetLookDir(const Vec2f& lookDir);

private:
  // Eye look dir: (0, 0) is dead ahead; (1, 0) is looking right, etc.
  Vec2f m_lookDir;
  Vec2f m_desiredLookDir;
};
}
