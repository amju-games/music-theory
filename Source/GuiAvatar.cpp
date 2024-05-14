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
#include "GuiAvatar.h"

namespace
{
const char* RIGHT_EYE = "right-eye";
const char* LEFT_EYE = "left-eye";
const char* PUPIL_TRANSLATE = "translate-pupil-and-iris";
const char* BLINK_ANIM = "blink";

const float MAX_BLINK_TIME = 0.2f;
const int BLINK_RATE = 5; // probability of blinking is 1 / this number
const float MAX_BLINK_CHECK_TIME = 0.5f; // decide to blink at this freq
} // anon namespace

namespace Amju
{
const char* GuiAvatar::NAME = "gui-avatar";

using AvatarFunc = std::function<void(GuiElement*, const std::string&)>;
using AvatarMap = std::map<std::string, AvatarFunc>;

// Map of gui type names to functions which set the attribute of that type.
// E.g. the string "colour" is mapped to a function which sets the value
//  of a GuiDecColour.
AvatarMap s_avatarMap = 
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

bool GuiAvatar::Load(File* f)
{
  if (!f->GetDataLine(&m_name))
  {
    f->ReportError("Expected GuiAvatar name.");
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

// Set Avatar from string.
// We set descendant nodes from a string like this:
// "colour; iris-colour; 005678 | scale; head-scale; 1.0, 1.2 | ... "
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

std::string GuiAvatar::GetString() const
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

  // Store so we can report out the strings in GetString()
  m_values[type + ";" + name] = value;

  AvatarFunc func = it->second;
  Assert(func);
  // For symmetrical features (currently just eyes), we need to find the left
  //  and right instances of the element, and call the same function on each.
  auto leftSide = GetElementByName(LEFT_EYE);
  Assert(leftSide); // GUI doesn't contain node with expected name
  auto elem = leftSide->GetElementByName(name);
  if (elem)
  {
    func(elem, value);
    // We expect to find the corresponding element on the right side.
    auto rightSide = GetElementByName(RIGHT_EYE);
    Assert(rightSide); // GUI doesn't contain node with expected name
    elem = rightSide->GetElementByName(name);
    Assert(elem); // element not on right side but is on left
    func(elem, value);
  }
  else
  {
    elem = GetElementByName(name); // search from the root node
    if (!elem)
    {
      ReportError("Couldn't find element '" + name + "'");
      return false;
    }
    func(elem, value);
  }
  return true;
}

void GuiAvatar::Update()
{
  GuiComposite::Update();
  float dt = TheTimer::Instance()->GetDt();

  // TODO TEMP TEST
  // Fixed sequence of look dirs - should be more intelligent
  const Vec2f EYE_LOOK[4] = 
  {
    { 1, 0 }, { 0, 0 }, { -0.5f, 0.5f }, { 0.5f, 0 }
  };
  static float lookTime = 0;
  lookTime += dt;
  if (lookTime > 1.f)
  {
    lookTime = 0;
    static int look = 0;
    m_lookDir = EYE_LOOK[look];
    look = (look + 1) % 4;
  }

  // Get eyes, update them
  // Blinking
  bool yesBlink = m_blinkTime > 0;
  if (yesBlink)
  {
    m_blinkTime -= dt;
  }

  // TODO Only check this after a fixed time, otherwise blinking
  //  depends on frame rate
  m_blinkCheckTime += dt;
  if (m_blinkCheckTime > MAX_BLINK_CHECK_TIME)
  {
    m_blinkCheckTime = 0;
    if (!yesBlink && ((rand() % BLINK_RATE) == 0))
    {
      m_blinkTime = MAX_BLINK_TIME;
    }
  }

  // Move from current look dir to desired look dir
  m_lookDir = Lerp(m_lookDir, m_desiredLookDir, dt);
  const float LOOK_DIR_SCALE = 0.05f;
  Vec2f pos(m_lookDir.x * LOOK_DIR_SCALE, m_lookDir.y * LOOK_DIR_SCALE);
  const char* EYE[2] = { RIGHT_EYE, LEFT_EYE };
  for (int i = 0; i < 2; i++)
  {
    auto eye = GetElementByName(EYE[i]);
    Assert(eye);
    auto pupil = dynamic_cast<GuiDecTranslate*>(eye->GetElementByName(PUPIL_TRANSLATE));
    Assert(pupil);
    // Compensate for scale of -1 in x for left eye (this is there so eye
    //  x-coords are symmetrical about centre line of face).
    if (i == 1)
    {
      pos = Vec2f(-pos.x, pos.y);
    }
    pupil->SetTranslation(pos);

    auto blink = dynamic_cast<GuiDecAnimation*>(eye->GetElementByName(BLINK_ANIM));
    Assert(blink);
    blink->SetEaseType(yesBlink ? Animator::EaseType::EASE_TYPE_ONE : Animator::EaseType::EASE_TYPE_ZERO);
  }
}

void GuiAvatar::SetLookDir(const Vec2f& lookDir)
{
  m_desiredLookDir = lookDir;
}
}

