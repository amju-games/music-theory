// * Amjula music theory *
// (c) Copyright 2024 Juliet Colman

#include <GuiDecAnimation.h>
#include <GuiDecColour.h>
#include <GuiDecRotate.h>
#include <GuiDecScale.h>
#include <GuiDecTranslate.h>
#include <GuiFactory.h>
#include <GuiSprite.h>
#include <Lerp.h>
#include <LoadVec2.h>
#include <ReportError.h>
#include <StringUtils.h>
#include <Timer.h>
#include "GuiSetFromString.h"

namespace Amju
{
const char* GuiSetFromString::NAME = "set-from-string";

// Map of gui type names to functions which set the attribute of that type.
// E.g. the string "colour" is mapped to a function which sets the value
//  of a GuiDecColour.
static const GuiSetFromString::SetFromStringMap s_avatarMap = 
{
  { 
    GuiDecColour::NAME, 
    [](GuiElement* elem, const std::string& value)
    {
      GuiDecColour* gc = dynamic_cast<GuiDecColour*>(elem);
      Colour col = *FromHexString(value);
      gc->SetColour(col);
      // set same colour again so no interpolation surprises
      gc->SetSecondColour(col);
    }
  },

  {
    GuiDecScale::NAME,
    [](GuiElement* elem, const std::string& value)
    {
      Vec2f scale;
      if (!ToVec2(value, &scale))
      {
        ReportError("Bad scale value for " + elem->GetName());
        Assert(false);
      }
      GuiDecScale* gs = dynamic_cast<GuiDecScale*>(elem);
      Assert(gs);
      gs->SetScale(scale);
      //gs->SetScale(scale, 1); // as for colour, set both ends of lerp
    }
  },

  {
    "pivot",
    [](GuiElement* elem, const std::string& value)
    {
      Vec2f pivot;
      if (!ToVec2(value, &pivot))
      {
        ReportError("Bad pivot value for " + elem->GetName());
        Assert(false);
      }
      GuiDecScale* scale = dynamic_cast<GuiDecScale*>(elem);
      if (scale)
      {
        scale->SetPivot(pivot);
      }
      else
      {
        GuiDecRotate* rot = dynamic_cast<GuiDecRotate*>(elem);
        if (rot)
        {
          rot->SetPivot(pivot);
        }
        else
        {
          ReportError("Can't set pivot on node: " + elem->GetName());
        }
      }
    }
  },

  {
    GuiDecTranslate::NAME,
    [](GuiElement* elem, const std::string& value)
    {
      Vec2f translation;
      if (!ToVec2(value, &translation))
      {
        ReportError("Bad translation value for " + elem->GetName());
        Assert(false);
      }
      GuiDecTranslate* gt = dynamic_cast<GuiDecTranslate*>(elem);
      Assert(gt);
      gt->SetTranslation(translation);
      gt->SetTranslation(translation, 1);
    }
  },

  {
    "cell",
    [](GuiElement* elem, const std::string& value)
    {
      GuiSprite* sprite = dynamic_cast<GuiSprite*>(elem);
      // Allow a single cell or a range, so check for a comma in value
      if (StringContains(value, ","))
      {
        Vec2i cellRange;
        ToVec2(value, &cellRange);
        sprite->SetCellRange(cellRange.x, cellRange.y);
      }
      else
      {
        int v = ToInt(value);
        sprite->SetCellRange(v, v);
      }
    }
  }
};

bool GuiSetFromString::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected GuiSetFromString name.");
    return false;
  }

  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected avatar string.");
    return false;
  }

  // Load descendants, which should contain the nodes named up at the top of 
  //  this file, e.g. LEFT_EYE etc.
  if (!GuiDecorator::Load(f))
  {
    return false;
  }

  if (!SetFromString(s))
  {
    f->ReportError("Bad avatar string.");
    return false;
  }

  return true;
}

// Set SetFromString from string.
// We set descendant nodes from a string like this:
// "colour; iris-colour; 005678 | scale; head-scale; 1.0, 1.2 | ... "
//  1. Split using |
//  2. This gives 3 strings separated with ;
//  3. First is type, so we know what to dynamic_cast to (or could use a factory ?)
//  4. Second is name of GuiElement we will set attribs on
//  5. Third is value of attrib - how we set it depends on the type (string 1), 
//     so again a factory could be the best way.
bool GuiSetFromString::SetFromString(const std::string& cs)
{
  if (cs == "$$$EMPTY")
  {
    return true;
  }
  Strings strs = Split(cs, '|');
  for (const std::string& s : strs)
  {
    if (!SetOneDescendant(s))
    {
      return false;
    }
  }
  return true;
}

std::string GuiSetFromString::GetString() const
{
  std::string result;
  for (auto it = m_values.begin(); it != m_values.end(); ++it)
  {
    result += it->first + ";" + it->second;
    if (std::distance(it, m_values.end()) > 1)
    {
      result += "|";
    }
  }
  return result;
}

bool GuiSetFromString::SetOneDescendant(const std::string& cs)
{
  Strings strs = Split(cs, ';');
  if (strs.size() != 3)
  {
    ReportError("Expected string to be in format \"type;name;value\": " + cs);
    return false;
  }
  const std::string& type = strs[0];
  const std::string& name = strs[1];
  const std::string& value = strs[2];

  // strs should be in format type;name;value.
  // We use the type to create an appropriate function, then call it with the 
  //  name and value. Inside the function, we'll know how to use the value.
  SetFromStringMap::const_iterator it = s_avatarMap.find(type);
  if (it == s_avatarMap.end())
  {
    return false;
  }

  // Store so we can report out the strings in GetString()
  m_values[type + ";" + name] = value;

  SetFromStringFunc func = it->second;
  Assert(func);
  return SetAttribOfNamedElementWithFunc(name, func, value);
}

bool GuiSetFromString::SetAttribOfNamedElementWithFunc(
  const std::string& name, SetFromStringFunc func, const std::string& value)
{
  auto elem = GetElementByName(name); // search from the root node
  if (!elem)
  {
    ReportError("Couldn't find element '" + name + "'");
    return false;
  }
  func(elem, value);
  return true;
}
}

