// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "Consts.h"
#include "Course.h"
#include "GSMainCorridor.h"
#include "GSTopicEnd.h"
#include "NumUpdate.h"
#include "Topic.h"
#include "UserProfile.h"

namespace Amju
{
namespace 
{
void OnOK(GuiElement*)
{
  TheMessageQueue::Instance()->Add(new FuncMsg(
    GoTo<TheGSMainCorridor>, SecondsFromNow(1.0f)));
}

void UpdateNums()
{
  GSTopicEnd* gte = TheGSTopicEnd::Instance();
  if (TheGame::Instance()->GetState() == gte)
  {
    TheGSTopicEnd::Instance()->UpdateNums();
  }
}
} // anon namespace

GSTopicEnd::GSTopicEnd()
{
  m_guiFilename = "Gui/gs_topic_end.txt";
  m_sceneFilename = "Scene/topic_start_scene.txt";
}

void GSTopicEnd::SetBestScore(int best)
{
  m_bestScore = best;
}

void GSTopicEnd::OnActive()
{
  GSBase3d::OnActive();

  // Set button commands
  GuiElement* elem = GetElementByName(m_gui, "ok-button");
  elem->SetCommand(OnOK);
  elem->SetHasFocus(true);

  GuiElement* shareButton = GetElementByName(m_gui, "share-button");
  Assert(shareButton);
  shareButton->SetCommand(Amju::OnShare);
  // TODO Set Share message and image
  
  auto profile = TheUserProfile();
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(profile->GetCurrentTopic());
  Assert(topic);

  // Get the non-persistent score for the current attempt. 
  m_topicScore = profile->GetCurrentTopicScore();

  // Unlock next topics if this topic has been passed.
  if (profile->IsTopicPassed(topic->GetId()))
  {
    // Passed this topic, unlock more topics
    const std::vector<std::string>& unlocks = topic->GetTopicsThisUnlocks();
    for (const std::string& id : unlocks)
    {
      profile->UnlockTopic(id);
    }
  }

  // Save all owned config files, but only for those that have changed.
  profile->Save();

  m_totalScore = 0; 

  // Set comment about score etc
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "comment-text"));
  Assert(text);
  text->SetText(GenerateScoreComment());

  // Set topic display name
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(profile->GetCurrentTopicDisplayName());

  // Get initial hints, animate additions
  m_hints = profile->GetHints(HintType::HINT_TYPE_HINT); // TODO

  // Add hints now
  int addHints = m_topicScore; // TODO calc num hints to add
  m_finalHints = m_hints + addHints;
  profile->AddHints(HintType::HINT_TYPE_HINT, addHints);  // TODO

  SetScoreNumbers();
  SetHintNumbers();

  // Set best so far %
  // If we just reached the best, show it with particles etc., once our score
  //  has finished incrementing.
  // If we are below the best so far, just show it from the start.

  int best = profile->GetBestTopicScore(topic->GetId());
  if (best > m_bestScore)
  {
    // We have beaten the previous best score!
    Assert(m_topicScore == best); // sanity check
  }
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "best-so-far-num"));
  Assert(text);
  text->SetText(ToString(m_bestScore) + "%");
  if (m_topicScore < best)
  {
    // We didn't beat the best score
    Assert(best == m_bestScore); // sanity check: best score is unchanged
  }

  // Send a timed message to do a number animation
  // First msg is delayed longer than subsequent msgs.
  TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(1.0f)));
}

std::string GSTopicEnd::GenerateScoreComment()
{
  std::string res = "Awesome";

  auto profile = TheUserProfile();
  Course* course = GetCourse();
  Assert(course);
  Topic* topic = course->GetTopic(profile->GetCurrentTopic());
  Assert(topic);

  bool passed = profile->IsTopicPassed(topic->GetId());

  if (passed)
  {
    res = "You passe the test!";
  }
  else
  {
    res = "Oh no! Try again!";
  }

  if (m_topicScore > m_bestScore)
  {
    res = "Your new best score!";
  }
  return res;
}

void GSTopicEnd::SetScoreNumbers()
{
  // ????
  NumUpdate(m_gui, "topic-score-text" /* TODO CONST */, m_topicScore);

  NumUpdate(m_gui, "total-score-text" /* TODO CONST */, 
    ToString(m_totalScore) + "%");

  NumUpdate(m_gui, "best-so-far-num" /* TODO CONST */, 
    ToString(m_bestScore) + "%");
}

void GSTopicEnd::SetHintNumbers()
{
  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, m_hints);
}

void GSTopicEnd::UpdateNums()
{
  // Set this to true if we need to do another update after this one
  bool updateAgain = true;

  if (m_topicScore > 0)
  {
    const int incr = 1;
    m_topicScore -= incr;
    if (m_topicScore <= 0)
    {
      m_topicScore = 0;
      updateAgain = false;
    }
    m_totalScore += incr;
    if (m_totalScore > m_bestScore)
    {
      m_bestScore = m_totalScore;
    }

    SetScoreNumbers();

    updateAgain = true;
  }

  // Hint counter
  if (m_hints < m_finalHints)
  {
    // TODO && we pass some multiple, e.g. every 100 points
    m_hints++;

    SetHintNumbers();
  }

  // Send a timed message to do this again soon
  // Only if nums are not showing their final values yet
  if (updateAgain)
  {
    TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(0.1f)));
  }
}
}

