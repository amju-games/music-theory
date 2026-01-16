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

bool GuiAvatar::SetAttribOfNamedElementWithFunc(
  const std::string& name, SetFromStringFunc func, const std::string& value)
{
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
  GuiSetFromString::Update();
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

