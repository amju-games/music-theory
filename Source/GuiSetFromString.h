// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#pragma once

#include <GuiDecorator.h>

namespace Amju
{
// * GuiDecSetAttribsFromString *
// Decorator which sets attributes on descendant nodes.
// This is so we can customise the look of a standard tree of sprites, but
//  could perhaps be used for other things too.
class GuiSetFromString : public GuiDecorator
{
public:
  using SetFromStringFunc = std::function<void(GuiElement*, const std::string&)>;
  using SetFromStringMap = std::map<std::string, SetFromStringFunc>;

  static const char* NAME;
  std::string GetTypeName() const override { return NAME; }

  bool Load(File*) override;

  // Set attrbutes in descendant nodes from the given string.
  // We set descendant nodes from a string like this:
  // "colour; iris - colour; 005678 | scale; head - scale; 1.0, 1.2 | ... "
  //  1. Split using |
  //  2. This gives 3 strings separated with ;
  //  3. First is type, so we know what to dynamic_cast to 
  //     (or could use a factory?)
  //  4. Second is name of GuiElement we will set attribs on
  //  5. Third is value of attrib - how we set it depends on the type (string 1). 
  bool SetFromString(const std::string&);

  // Get string of attribute settings,  which can be passed in to SetFromString()
  std::string GetString() const;

  // Call this to set a value on one descendant node. This is used when we set
  //  from string, and can be used to animate or alter attribs later.
  // Format of string is "type;name;value", e.g. "colour; iris-colour; 005678"
  // The type should use names used by GuiFactory where possible, for 
  //  consistency - no point having a second set of different names for things.
  // Name is the name of the GuiElement. 
  // Value depends on the type, e.g. for a colour, it's a hex rgb[a] value.
  bool SetOneDescendant(const std::string&);

protected:
  // Set the named element (which should be a descendant), using the given
  //  func (which is appropriate to the type of the named element) to
  //  set the new value.
  // Override to e.g. set symmetrical instances of elements with the
  //  same name.
  // Returns true if we successfully find the element and set its value
  //  using the given function.
  virtual bool SetAttribOfNamedElementWithFunc(
    const std::string& name, SetFromStringFunc func, const std::string& value);

protected:
  // Map of strings set with SetFromString() and SetOneDescendant(). Map so we
  //  only store the last string set for a given type + name.
  // We use this map so we can report it out in GetString().
  std::map<std::string, std::string> m_values;
};
}
