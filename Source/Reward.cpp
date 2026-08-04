#include <iostream>
#include <AmjuRand.h>
#include <GuiDecAnimation.h>
#include "Extra.h"
#include "GSHero.h"
#include "GuiDecCurvedPath.h"
#include "Reward.h"

namespace Amju
{
// Callback: called when reward animation reaches destination.
static void OnRewardAnimComplete(Animator* animator)
{
  Assert(animator);
  // GuiDecAnimation is an Animator
  GuiDecAnimation* guiAnim = dynamic_cast<GuiDecAnimation*>(animator);
  Assert(guiAnim);
  // We had set user data in IReward::StartCollectAnim to point to the Reward 
  void* data = guiAnim->GetUserData();
  Assert(data);
  IReward* reward = reinterpret_cast<IReward*>(data);
  // Notify reward it has reached the end of its anim
  reward->OnDestPosReached();
}

// Callback: called when the animation for NOT collecting the reward
//  is complete.
static void OnRewardNoCollectAnimComplete(Animator* animator)
{
  Assert(animator);
  // GuiDecAnimation is an Animator
  GuiDecAnimation* guiAnim = dynamic_cast<GuiDecAnimation*>(animator);
  Assert(guiAnim);
  // We had set user data in IReward::StartNoCollectAnim to point to the Reward 
  void* data = guiAnim->GetUserData();
  Assert(data);
  IReward* reward = reinterpret_cast<IReward*>(data);
  // Notify reward it has reached the end of its anim
  reward->OnNoCollectAnimComplete();
}

void IReward::OnNoCollectAnimComplete()
{
  Assert(m_parent);
  m_parent->FinishNoCollect();
}

PGuiElement IReward::GetGui()
{
  Assert(m_parent);
  return m_parent->GetGui();
}

void IReward::OnDestPosReached()
{
  // Gui detaches from parent (root)..
  // Would this be better done in IExtra::Collect?
  auto gui = GetGui();
  Assert(gui);
  auto parent = gui->GetParent();
  Assert(parent);
  auto comp = dynamic_cast<GuiComposite*>(parent);
  Assert(comp);
  m_parent->DetachGui(comp);

  // Give reward: this depends on the extra type so we go round the houses
  //  a bit here.
  m_parent->Collect(); // (Extra owner of this reward, not parent gui node)
}

// Pause the given animation controller node, and return the casted node.
static GuiDecAnimation* PauseAnim(GuiElement* elem)
{
  Assert(elem);
  auto animController = dynamic_cast<GuiDecAnimation*>(elem);
  Assert(animController);
  animController->SetIsPaused(true);
  return animController;
}

void IReward::InitAnim()
{
  // We should have a GUI and that should contain an animation controller.
  // Set the anim to initially paused.
  auto gui = GetGui();
  Assert(gui);

  // Set anims to paused 
  // This is the anim for the curved path if we collect 
  auto elem = GetElementByName(gui, "extra-anim-controller-collect");
  m_animControllerCollect = PauseAnim(elem);

  // This is the anim for if we don't collect
  elem = GetElementByName(gui, "extra-anim-controller-no-collect");
  m_animControllerNoCollect = PauseAnim(elem);
}

static void RandomiseCurvedPath(PathConfig& config)
{
  // Randomise the path
  config.spiral.maxRadius = Rnd(.4f, .8f); 
}
 
void IReward::StartNoCollectAnim()
{
  // Start the 'no collect' animation
  m_animControllerNoCollect->SetIsPaused(false); 

  // Set up 'On no-collect anim finished' callback
  m_animControllerNoCollect->SetUserData(this); // so we know which reward to notify
  m_animControllerNoCollect->SetOnCompleteCallback(OnRewardNoCollectAnimComplete);
}

void IReward::StartCollectAnim()
{
  auto gui = GetGui();
  Assert(gui);
  // Set start and dest pos in curve config
  auto elem = GetElementByName(gui, "curved-path-for-heart");
  Assert(elem);
  auto curve = dynamic_cast<GuiDecCurvedPath*>(elem);
  Assert(curve);
  auto config = curve->GetPathConfig();
  // Get pos of extra gui now: this becomes the start pos of the curve.
  // Once set, the curve takes over from the local pos of the gui root.
  config.startPos = gui->GetLocalPos(); 
  // Set destination
  config.endPos = Vec2f(-.9, .7); //m_destPos; // TODO TEMP TEST
  RandomiseCurvedPath(config);
  curve->SetPathConfig(config);
  // We need this: we don't want curve pos + random extra pos
  gui->SetLocalPos(Vec2f(0, 0)); // curve takes over setting position

  // Start the collect animation!
  m_animControllerCollect->SetIsPaused(false);

  // Set up 'On anim finished' callback
  m_animControllerCollect->SetUserData(this); // so we know which reward to notify
  m_animControllerCollect->SetOnCompleteCallback(OnRewardAnimComplete);
}

void RewardHealth::GiveReward()
{
#ifdef EXTRA_DEBUG
std::cout << "Give reward: " << m_points << " health points!\n";
#endif

  TheGSHero::Instance()->IncreaseLife(m_points);
}
}

