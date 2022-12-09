// * Amjula music theory *
// (c) Copyright 2017 Jason Colman

#include <Game.h>
#include <GuiText.h>
#include <MessageQueue.h>
#include "GSMainCorridor.h"
#include "GSTopicEnd.h"
#include "NumUpdate.h"
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

  m_topicScore = profile->GetTopicScore();
  m_totalScore = profile->GetScore();

  // Set comment about score etc
  IGuiText* text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "comment-text"));
  Assert(text);
  text->SetText(GenerateScoreComment());

  // Set topic display name
  text = dynamic_cast<IGuiText*>(GetElementByName(m_gui, "topic-name-text"));
  Assert(text);
  text->SetText(profile->GetCurrentTopicDisplayName());

  // Add topic score to persistent total score now, but animate the
  //  transfer of numbers, and add to hints over time.
  profile->AddTopicScoreToPersistentScore();

  // Get initial hints, animate additions
  m_hints = profile->GetHints();

  // Add hints now
  int addHints = m_topicScore; // TODO calc num hints to add
  m_finalHints = m_hints + addHints;
  profile->AddHints(addHints); 

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
  NumUpdate(m_gui, "total-score-text" /* TODO CONST */, m_totalScore);
}

void GSTopicEnd::SetHintNumbers()
{
  NumUpdate(m_gui, "hint-counter" /* TODO CONST */, m_hints);
}

void GSTopicEnd::UpdateNums()
{
  // Set this to true if we need to do another update after this one
  bool updateAgain = false;

  if (m_topicScore > 0)
  {
    const int incr = 1;
    m_topicScore -= incr;
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

    updateAgain = true;
  }

  // Send a timed message to do this again soon
  // Only if nums are not showing their final values yet
  if (updateAgain)
  {
    TheMessageQueue::Instance()->Add(new FuncMsg(::Amju::UpdateNums, SecondsFromNow(0.2f)));
  }
}
}

