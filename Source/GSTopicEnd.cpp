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
}

void GSTopicEnd::OnActive()
{
  // This works well because it skips loading scene again in GSBase3d::OnActive
  GSBase::OnActive();

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

  // Config file specifically for storing topic-related stuff
  ////auto topicConfig = profile->GetConfigForTopic(KEY_TOPICS);

  // Get the non-persistent score for the current attempt. 
  m_topicScore = profile->GetCurrentTopicScore();

  // Set persistent best mark if current score is better than previous best
  ////int best = topicConfig->GetInt(KEY_TOPIC_BEST + id, 0);
  ////if (m_topicScore > best)
  ////{
  ////  topicConfig->SetInt(KEY_TOPIC_BEST + id, m_topicScore);
  ////}
  
  // Unlock next topics if this topic has been passed.
  if (profile->IsTopicPassed(topic->GetId()))
      ////m_topicScore >= topic->GetPassMark())
  {
    // Passed this topic, unlock more topics
    const std::vector<std::string>& unlocks = topic->GetTopicsThisUnlocks();
    for (const std::string& id : unlocks)
    {
      ////topicConfig->SetInt(KEY_TOPIC_UNLOCKED + id, 1);
      profile->UnlockTopic(id);
    }
  }

  // Save all owned config files, but only for those that have changed.
  // Right?
  // TODO Check this
  profile->Save();

  m_totalScore = 0; // profile->GetScore();

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

  // Send a timed message to do a number animation
  // First msg is delayed longer than subsequent msgs.
  TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(1.0f)));
}

std::string GSTopicEnd::GenerateScoreComment()
{
  std::string res = "Awesome";
  return res;
}

void GSTopicEnd::SetScoreNumbers()
{
  NumUpdate(m_gui, "topic-score-text" /* TODO CONST */, m_topicScore);
  NumUpdate(m_gui, "total-score-text" /* TODO CONST */, 
    ToString(m_totalScore) + "%");
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

