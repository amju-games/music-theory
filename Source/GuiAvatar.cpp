// * Amjula music theory *
// (c) Copyright 2017-2018 Jason Colman

#include <GuiDecColour.h>
#include <GuiDecScale.h>
#include <GuiDecTranslate.h>
#include <GuiFactory.h>
#include <GuiImage.h>
#include <LoadVec2.h>
#include <ReportError.h>
#include <StringUtils.h>
#include <Timer.h>
#include "GuiAvatar.h"

namespace
{
const char* RIGHT_EYE = "right-eye";
const char* LEFT_EYE = "left-eye";
const char* PUPIL = "translate-scale-pupil";

} // anon namespace

namespace Amju
{
using AvatarFunc = std::function<void(GuiElement*, const std::string&)>;
using AvatarMap = std::map<std::string, AvatarFunc>;
AvatarMap s_avatarMap = 
{
  { 
    GuiDecColour::NAME, 
    [](GuiElement* elem, const std::string& value)
    {
      dynamic_cast<GuiDecColour*>(elem)->SetColour(FromHexString(value));
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
      dynamic_cast<GuiDecScale*>(elem)->SetScale(scale);
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
      dynamic_cast<GuiDecScale*>(elem)->SetPivot(pivot);
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
      dynamic_cast<GuiDecTranslate*>(elem)->SetTranslation(translation);
    }
  },

  {
    GuiImage::NAME,
    [](GuiElement* elem, const std::string& value)
    {
      Texture* tex = (Texture*)TheResourceManager::Instance()->GetRes(value);
      dynamic_cast<GuiImage*>(elem)->SetTexture(tex);
    }
  },
};

void GuiAvatar::AddToFactory()
{
  TheGuiFactory::Instance()->Add("gui-avatar", []()->GuiElement*
  {
    return new GuiAvatar;
  });
}

bool GuiAvatar::Load(File* f)
{
  if (!GuiComposite::Load(f))
  {
    return false;
  }
  std::string s;
  if (!f->GetDataLine(&s))
  {
    f->ReportError("Expected avatar string.");
    return false;
  }
  if (!SetFromString(s))
  {
    f->ReportError("Bad avatar string.");
    return false;
  }
  return true;
}

// Set Avatar from string.
// We set descendant nodes from a string like this:
// "colour; iris - colour; 005678 | scale; head - scale; 1.0, 1.2 | ... "
//  1. Split using |
//  2. This gives 3 strings separated with ;
//  3. First is type, so we know what to dynamic_cast to (or could use a factory ?)
//  4. Second is name of GuiElement we will set attribs on
//  5. Third is value of attrib - how we set it depends on the type (string 1), 
//     so again a factory could be the best way.
bool GuiAvatar::SetFromString(const std::string& cs)
{
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

bool GuiAvatar::SetOneDescendant(const std::string& cs)
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
  AvatarMap::iterator it = s_avatarMap.find(type);
  if (it == s_avatarMap.end())
  {
    return false;
  }
  AvatarFunc func = it->second;
  Assert(func);
  auto elem = GetElementByName(name);
  if (!elem)
  {
    ReportError("Couldn't find element '" + name + "'");
    return false;
  }
  func(elem, value);
  return true;
}

void GuiAvatar::Update()
{
  GuiComposite::Update();
  float dt = TheTimer::Instance()->GetDt();
  // Get eyes, update them
  // Move from current look dir to desired look dir?
  m_lookDir = m_desiredLookDir;
  const float LOOK_DIR_SCALE = 0.05f;
  Vec2f pos(m_lookDir.x * LOOK_DIR_SCALE, m_lookDir.y * LOOK_DIR_SCALE);
  const char* EYE[2] = { RIGHT_EYE, LEFT_EYE };
  for (int i = 0; i < 2; i++)
  {
    auto eye = GetElementByName(EYE[i]);
    Assert(eye);
    auto pupil = dynamic_cast<GuiDecTranslate*>(eye->GetElementByName(PUPIL));
    Assert(pupil);
    pupil->SetTranslation(pos);
  }
}

void GuiAvatar::SetLookDir(const Vec2f& lookDir)
{
  m_desiredLookDir = lookDir;
}
}

